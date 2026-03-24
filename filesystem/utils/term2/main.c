#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/task.h>
#include <sys/io.h>
#include <sys/vp.h>
#include <sys/dir.h>
#include <sys/console.h>
#include <sys/memory.h>
#include <sys/stat.h>
#include <exec/exec.h>
#include <string.h>

#define TERM_W 80
#define TERM_H 24

#define TERM_MAX_W 220
#define TERM_MAX_H 220

#define CHAR_SCALE 8

#define ANSI_MAX_PARAMS 8

typedef enum {
    ANSI_STATE_NORMAL,   /* regular text                      */
    ANSI_STATE_ESC,      /* received ESC (0x1B)               */
    ANSI_STATE_CSI,      /* received ESC [  →  collecting CSI */
    ANSI_STATE_OSC,      /* received ESC ]  →  skip until ST  */
} AnsiState;

struct AnsiParser {
    AnsiState   state;
    int         params[ANSI_MAX_PARAMS];
    int         param_count;
    int         cur_param;           /* accumulator for the digit being built */
    int         has_cur_param;       /* flag: cur_param is valid              */
    char        intermediate;        /* single intermediate byte (0x20-0x2F) */
};

/* ──────────────────────────────────────────────
 * Cell-based screen buffer
 *
 * Each cell stores a character, a foreground and
 * a background colour.  This replaces the flat
 * char *text_buf and makes attribute changes
 * (colour, inverse-video…) straightforward.
 * ────────────────────────────────────────────── */
typedef struct {
    char     ch;
    uint32_t fg;
    uint32_t bg;
} Cell;

/* ──────────────────────────────────────────────
 * Terminal
 * ────────────────────────────────────────────── */
struct Terminal {
    /* logical dimensions (columns × rows, *including* border) */
    int term_w;
    int term_h;
    int char_size;

    /* cell buffer – term_w * term_h entries */
    Cell *cells;

    /* cursor position in the *printable* area
       (0-based, [0 .. term_w-2] × [0 .. term_h-2]) */
    int cursor_x;
    int cursor_y;

    /* current text attributes */
    uint32_t attr_fg;
    uint32_t attr_bg;

    /* ANSI parser */
    struct AnsiParser ansi;

    /* pixel buffer / viewport */
    int      vp_w;
    int      vp_h;
    uint32_t bufsize;
    uint32_t *buf;
    struct Viewport *vp;
};

struct Terminal *g_term;

/* ──────────────────────────────────────────────
 * Forward declarations
 * ────────────────────────────────────────────── */
void term_resize(struct Terminal *term, int vp_w, int vp_h);
void term_draw(struct Terminal *term);
static void term_process_byte(struct Terminal *term, unsigned char c);

/* ──────────────────────────────────────────────
 * Viewport event handler
 * ────────────────────────────────────────────── */
void term_ev_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event) {
    switch (event) {
        case VP_RESIZE:
            term_resize(g_term, vp->resizeLoc.w, vp->resizeLoc.h);
            vp->loc.w = g_term->vp_w;
            vp->loc.h = g_term->vp_h;
            term_draw(g_term);
            break;
    }
}

/* ──────────────────────────────────────────────
 * Cell buffer helpers
 * ────────────────────────────────────────────── */

/* Printable area: columns 0..term_w-2, rows 0..term_h-2
   (column term_w-1 and row term_h-1 are reserved for the border). */
#define PRINT_W(t) ((t)->term_w - 1)
#define PRINT_H(t) ((t)->term_h - 1)

static inline Cell *cell_at(struct Terminal *term, int x, int y) {
    return &term->cells[y * term->term_w + x];
}

static void cells_clear_region(struct Terminal *term,
                                int x0, int y0, int x1, int y1) {
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            Cell *c = cell_at(term, x, y);
            c->ch = ' ';
            c->fg = term->attr_fg;
            c->bg = term->attr_bg;
        }
    }
}

static void cells_clear_all(struct Terminal *term) {
    cells_clear_region(term, 0, 0, PRINT_W(term) - 1, PRINT_H(term) - 1);
}

/* ──────────────────────────────────────────────
 * Scroll: move rows [1..PRINT_H-1] up by one,
 * blank the bottom row.
 * ────────────────────────────────────────────── */
static void term_scroll_up(struct Terminal *term) {
    int pw = PRINT_W(term);
    int ph = PRINT_H(term);
    /* shift rows up */
    memmove(&term->cells[0],
            &term->cells[term->term_w],
            sizeof(Cell) * term->term_w * (ph - 1));
    /* blank the new bottom row */
    cells_clear_region(term, 0, ph - 1, pw - 1, ph - 1);
}

/* ──────────────────────────────────────────────
 * Cursor movement (clamped to printable area)
 * ────────────────────────────────────────────── */
static void cursor_clamp(struct Terminal *term) {
    int pw = PRINT_W(term);
    int ph = PRINT_H(term);
    if (term->cursor_x < 0)   term->cursor_x = 0;
    if (term->cursor_x >= pw) term->cursor_x = pw - 1;
    if (term->cursor_y < 0)   term->cursor_y = 0;
    if (term->cursor_y >= ph) term->cursor_y = ph - 1;
}

/* Advance cursor one position, wrapping and scrolling as needed. */
static void cursor_advance(struct Terminal *term) {
    int pw = PRINT_W(term);
    int ph = PRINT_H(term);
    term->cursor_x++;
    if (term->cursor_x >= pw) {
        term->cursor_x = 0;
        term->cursor_y++;
        if (term->cursor_y >= ph) {
            term_scroll_up(term);  /* scroll */
            term->cursor_y = ph - 1;
        }
    }
}

/* ──────────────────────────────────────────────
 * Drawing
 * ────────────────────────────────────────────── */
static void term_draw_border(struct Terminal *term) {
    int cs = term->char_size;
    for (int x = 0; x < term->term_w; x++)
        vp_drawChar(term->vp, x * cs, (term->term_h - 1) * cs,
                    '-', 0x000000, 0xFFFFFF);
    for (int y = 0; y < term->term_h; y++)
        vp_drawChar(term->vp, (term->term_w - 1) * cs, y * cs,
                    '|', 0x000000, 0xFFFFFF);
}

void term_draw(struct Terminal *term) {
    memset(term->buf, 0, term->bufsize);
    int cs = term->char_size;
    int pw = PRINT_W(term);
    int ph = PRINT_H(term);

    for (int y = 0; y < ph; y++) {
        for (int x = 0; x < pw; x++) {
            Cell *c = cell_at(term, x, y);
            int is_cursor = (x == term->cursor_x && y == term->cursor_y);
            uint32_t fg = is_cursor ? c->bg : c->fg;
            uint32_t bg = is_cursor ? c->fg : c->bg;
            vp_drawChar(term->vp, x * cs, y * cs, c->ch ? c->ch : ' ', fg, bg);
        }
    }
    term_draw_border(term);
    vp_copy(term->vp);
}

/* ──────────────────────────────────────────────
 * Resize
 * ────────────────────────────────────────────── */
void term_resize(struct Terminal *term, int vp_w, int vp_h) {
    int new_w = (vp_w / term->char_size) + 1;
    int new_h = (vp_h / term->char_size) + 1;

    /* clamp */
    if (new_w > TERM_MAX_W) new_w = TERM_MAX_W;
    if (new_h > TERM_MAX_H) new_h = TERM_MAX_H;

    term->term_w = new_w;
    term->term_h = new_h;

    term->vp_w = term->term_w * term->char_size;
    term->vp_h = term->term_h * term->char_size;

    printf("Requested %dx%d - Term %dx%d - Target %dx%d\n",
           vp_w, vp_h, term->term_w, term->term_h, term->vp_w, term->vp_h);

    free(term->cells);
    term->cells = calloc(term->term_w * term->term_h, sizeof(Cell));
    cells_clear_all(term);

    cursor_clamp(term);

    memory_returnRegion(term->buf, term->bufsize);
    term->bufsize = sizeof(uint32_t) * term->vp_w * term->vp_h;
    term->buf     = memory_requestRegion(term->bufsize);
    vp_set_buffer(term->vp, term->buf, term->bufsize);

    term_draw(term);
    vp_copy(term->vp);
    set_schedule(ALWAYS);
}

/* ──────────────────────────────────────────────
 * ANSI / VT-100 escape code handling
 * ────────────────────────────────────────────── */

/* Flush any pending numeric parameter into the params array. */
static void ansi_flush_param(struct AnsiParser *p) {
    if (!p->has_cur_param) return;
    if (p->param_count < ANSI_MAX_PARAMS)
        p->params[p->param_count++] = p->cur_param;
    p->cur_param    = 0;
    p->has_cur_param = 0;
}

/* Return param[n] or def if not present. */
static int ansi_param(struct AnsiParser *p, int n, int def) {
    if (n >= p->param_count) return def;
    int v = p->params[n];
    return (v == 0) ? def : v;  /* 0 means "default" in most sequences */
}

/* SGR: Select Graphic Rendition – ESC [ <params> m */
static void term_handle_sgr(struct Terminal *term) {
    struct AnsiParser *p = &term->ansi;
    if (p->param_count == 0) {
        /* ESC [ m  →  reset all attributes */
        term->attr_fg = 0xFFFFFF;
        term->attr_bg = 0x000000;
        return;
    }
    for (int i = 0; i < p->param_count; i++) {
        switch (p->params[i]) {
            case 0:  term->attr_fg = 0xFFFFFF; term->attr_bg = 0x000000; break;
            case 7:  { uint32_t tmp = term->attr_fg;   /* reverse video */
                        term->attr_fg = term->attr_bg;
                        term->attr_bg = tmp; } break;
            /* Standard 8 foreground colours (30-37) */
            case 30: term->attr_fg = 0x000000; break;
            case 31: term->attr_fg = 0xAA0000; break;
            case 32: term->attr_fg = 0x00AA00; break;
            case 33: term->attr_fg = 0xAA5500; break;
            case 34: term->attr_fg = 0x0000AA; break;
            case 35: term->attr_fg = 0xAA00AA; break;
            case 36: term->attr_fg = 0x00AAAA; break;
            case 37: term->attr_fg = 0xAAAAAA; break;
            case 39: term->attr_fg = 0xFFFFFF; break;  /* default fg */
            /* Bright / high-intensity foreground (90-97) */
            case 90: term->attr_fg = 0x555555; break;
            case 91: term->attr_fg = 0xFF5555; break;
            case 92: term->attr_fg = 0x55FF55; break;
            case 93: term->attr_fg = 0xFFFF55; break;
            case 94: term->attr_fg = 0x5555FF; break;
            case 95: term->attr_fg = 0xFF55FF; break;
            case 96: term->attr_fg = 0x55FFFF; break;
            case 97: term->attr_fg = 0xFFFFFF; break;
            /* Standard 8 background colours (40-47) */
            case 40: term->attr_bg = 0x000000; break;
            case 41: term->attr_bg = 0xAA0000; break;
            case 42: term->attr_bg = 0x00AA00; break;
            case 43: term->attr_bg = 0xAA5500; break;
            case 44: term->attr_bg = 0x0000AA; break;
            case 45: term->attr_bg = 0xAA00AA; break;
            case 46: term->attr_bg = 0x00AAAA; break;
            case 47: term->attr_bg = 0xAAAAAA; break;
            case 49: term->attr_bg = 0x000000; break;  /* default bg */
            /* Bright background (100-107) */
            case 100: term->attr_bg = 0x555555; break;
            case 101: term->attr_bg = 0xFF5555; break;
            case 102: term->attr_bg = 0x55FF55; break;
            case 103: term->attr_bg = 0xFFFF55; break;
            case 104: term->attr_bg = 0x5555FF; break;
            case 105: term->attr_bg = 0xFF55FF; break;
            case 106: term->attr_bg = 0x55FFFF; break;
            case 107: term->attr_bg = 0xFFFFFF; break;
            /* 256-colour and RGB modes (38 / 48) are partially decoded below */
            case 38:
            case 48: {
                /* ESC [ 38 ; 2 ; R ; G ; B m  →  24-bit colour
                   ESC [ 38 ; 5 ; N m          →  256-colour (not mapped here) */
                if (i + 1 < p->param_count && p->params[i + 1] == 2
                        && i + 4 < p->param_count) {
                    uint32_t colour = ((uint32_t)p->params[i+2] << 16)
                                    | ((uint32_t)p->params[i+3] <<  8)
                                    |  (uint32_t)p->params[i+4];
                    if (p->params[i] == 38) term->attr_fg = colour;
                    else                    term->attr_bg = colour;
                    i += 4;
                }
                break;
            }
            default: break;
        }
    }
}

static void term_dispatch_csi(struct Terminal *term, char final) {
    struct AnsiParser *p = &term->ansi;
    ansi_flush_param(p);

    int pw = PRINT_W(term);
    int ph = PRINT_H(term);

    switch (final) {
        /* ── Cursor movement ── */
        case 'A': /* CUU – cursor up */
            term->cursor_y -= ansi_param(p, 0, 1);
            cursor_clamp(term);
            break;
        case 'B': /* CUD – cursor down */
            term->cursor_y += ansi_param(p, 0, 1);
            cursor_clamp(term);
            break;
        case 'C': /* CUF – cursor forward (right) */
            term->cursor_x += ansi_param(p, 0, 1);
            cursor_clamp(term);
            break;
        case 'D': /* CUB – cursor back (left) */
            term->cursor_x -= ansi_param(p, 0, 1);
            cursor_clamp(term);
            break;
        case 'E': /* CNL – cursor next line */
            term->cursor_y += ansi_param(p, 0, 1);
            term->cursor_x  = 0;
            cursor_clamp(term);
            break;
        case 'F': /* CPL – cursor previous line */
            term->cursor_y -= ansi_param(p, 0, 1);
            term->cursor_x  = 0;
            cursor_clamp(term);
            break;
        case 'G': /* CHA – cursor horizontal absolute */
            term->cursor_x = ansi_param(p, 0, 1) - 1;
            cursor_clamp(term);
            break;
        case 'H': /* CUP – cursor position  ESC [ row ; col H */
        case 'f': {
            int row = ansi_param(p, 0, 1) - 1;
            int col = ansi_param(p, 1, 1) - 1;
            term->cursor_y = row;
            term->cursor_x = col;
            cursor_clamp(term);
            break;
        }
        case 'd': /* VPA – vertical position absolute */
            term->cursor_y = ansi_param(p, 0, 1) - 1;
            cursor_clamp(term);
            break;

        /* ── Erase ── */
        case 'J': { /* ED – erase in display */
            int mode = (p->param_count > 0) ? p->params[0] : 0;
            switch (mode) {
                case 0: /* cursor to end */
                    cells_clear_region(term,
                                       term->cursor_x, term->cursor_y,
                                       pw - 1, term->cursor_y);
                    cells_clear_region(term,
                                       0, term->cursor_y + 1,
                                       pw - 1, ph - 1);
                    break;
                case 1: /* start to cursor */
                    cells_clear_region(term,
                                       0, 0,
                                       pw - 1, term->cursor_y - 1);
                    cells_clear_region(term,
                                       0, term->cursor_y,
                                       term->cursor_x, term->cursor_y);
                    break;
                case 2: /* entire screen */
                case 3: /* entire screen + scrollback (we have none) */
                    cells_clear_all(term);
                    break;
            }
            break;
        }
        case 'K': { /* EL – erase in line */
            int mode = (p->param_count > 0) ? p->params[0] : 0;
            switch (mode) {
                case 0: /* cursor to end of line */
                    cells_clear_region(term,
                                       term->cursor_x, term->cursor_y,
                                       pw - 1, term->cursor_y);
                    break;
                case 1: /* start of line to cursor */
                    cells_clear_region(term,
                                       0, term->cursor_y,
                                       term->cursor_x, term->cursor_y);
                    break;
                case 2: /* entire line */
                    cells_clear_region(term,
                                       0, term->cursor_y,
                                       pw - 1, term->cursor_y);
                    break;
            }
            break;
        }

        /* ── Insert / Delete ── */
        case 'L': { /* IL – insert lines */
            int n = ansi_param(p, 0, 1);
            int y = term->cursor_y;
            if (y < ph) {
                int lines_to_move = ph - y - n;
                if (lines_to_move > 0)
                    memmove(cell_at(term, 0, y + n),
                            cell_at(term, 0, y),
                            sizeof(Cell) * term->term_w * lines_to_move);
                cells_clear_region(term, 0, y, pw - 1,
                                   y + n - 1 < ph - 1 ? y + n - 1 : ph - 1);
            }
            break;
        }
        case 'M': { /* DL – delete lines */
            int n = ansi_param(p, 0, 1);
            int y = term->cursor_y;
            if (y < ph) {
                int lines_to_move = ph - y - n;
                if (lines_to_move > 0)
                    memmove(cell_at(term, 0, y),
                            cell_at(term, 0, y + n),
                            sizeof(Cell) * term->term_w * lines_to_move);
                int blank_start = y + lines_to_move;
                cells_clear_region(term, 0, blank_start, pw - 1, ph - 1);
            }
            break;
        }
        case 'P': { /* DCH – delete characters */
            int n = ansi_param(p, 0, 1);
            int y = term->cursor_y;
            int x = term->cursor_x;
            int chars_to_move = pw - x - n;
            if (chars_to_move > 0)
                memmove(cell_at(term, x, y),
                        cell_at(term, x + n, y),
                        sizeof(Cell) * chars_to_move);
            cells_clear_region(term, pw - n, y, pw - 1, y);
            break;
        }
        case '@': { /* ICH – insert blank characters */
            int n = ansi_param(p, 0, 1);
            int y = term->cursor_y;
            int x = term->cursor_x;
            int chars_to_move = pw - x - n;
            if (chars_to_move > 0)
                memmove(cell_at(term, x + n, y),
                        cell_at(term, x, y),
                        sizeof(Cell) * chars_to_move);
            cells_clear_region(term, x, y, x + n - 1, y);
            break;
        }

        /* ── Scroll ── */
        case 'S': { /* SU – scroll up */
            int n = ansi_param(p, 0, 1);
            for (int i = 0; i < n; i++) term_scroll_up(term);
            break;
        }
        case 'T': { /* SD – scroll down (insert blank at top) */
            int n = ansi_param(p, 0, 1);
            for (int i = 0; i < n; i++) {
                memmove(cell_at(term, 0, 1),
                        cell_at(term, 0, 0),
                        sizeof(Cell) * term->term_w * (ph - 1));
                cells_clear_region(term, 0, 0, pw - 1, 0);
            }
            break;
        }

        /* ── Attributes ── */
        case 'm':
            term_handle_sgr(term);
            break;

        /* ── Device status / cursor position report ── */
        case 'n':
            /* DSR: ESC [ 6 n  →  report cursor position.
               We write back ESC [ row ; col R to the child's stdin. */
            /* Not implemented: requires access to the write-end of stdin pipe.
               Left as a future extension. */
            break;

        default:
            /* Silently ignore unknown sequences. */
            break;
    }
}

static void term_process_byte(struct Terminal *term, unsigned char c) {
    struct AnsiParser *p = &term->ansi;

    switch (p->state) {

        /* ── Normal text ── */
        case ANSI_STATE_NORMAL:
            switch (c) {
                case 0x1B: /* ESC */
                    p->state = ANSI_STATE_ESC;
                    break;
                case '\r':
                    term->cursor_x = 0;
                    break;
                case '\n':
                    term->cursor_x = 0;
                    term->cursor_y++;
                    if (term->cursor_y >= PRINT_H(term)) {
                        term_scroll_up(term);
                        term->cursor_y = PRINT_H(term) - 1;
                    }
                    break;
                case '\t': {
                    /* advance to the next 8-column tab stop */
                    int next = (term->cursor_x / 8 + 1) * 8;
                    while (term->cursor_x < next && term->cursor_x < PRINT_W(term) - 1)
                        cursor_advance(term);
                    break;
                }
                case 0x08: /* BS – backspace */
                    if (term->cursor_x > 0) {
                        term->cursor_x--;
                        Cell *cel = cell_at(term, term->cursor_x, term->cursor_y);
                        cel->ch = ' ';
                        cel->fg = term->attr_fg;
                        cel->bg = term->attr_bg;
                    }
                    break;
                case 0x7F: /* DEL – ignored */
                    break;
                default:
                    if (c >= 0x20) { /* printable */
                        Cell *cel = cell_at(term, term->cursor_x, term->cursor_y);
                        cel->ch = (char)c;
                        cel->fg = term->attr_fg;
                        cel->bg = term->attr_bg;
                        cursor_advance(term);
                    }
                    break;
            }
            break;

        /* ── Received ESC ── */
        case ANSI_STATE_ESC:
            switch (c) {
                case '[': /* CSI */
                    p->state        = ANSI_STATE_CSI;
                    p->param_count  = 0;
                    p->cur_param    = 0;
                    p->has_cur_param = 0;
                    p->intermediate  = 0;
                    break;
                case ']': /* OSC – skip until ST or BEL */
                    p->state = ANSI_STATE_OSC;
                    break;
                case 'c': /* RIS – full reset */
                    term->attr_fg = 0xFFFFFF;
                    term->attr_bg = 0x000000;
                    term->cursor_x = 0;
                    term->cursor_y = 0;
                    cells_clear_all(term);
                    p->state = ANSI_STATE_NORMAL;
                    break;
                case 'M': /* RI – reverse index (scroll down one line) */
                    if (term->cursor_y > 0) {
                        term->cursor_y--;
                    } else {
                        /* at top: push content down */
                        memmove(cell_at(term, 0, 1),
                                cell_at(term, 0, 0),
                                sizeof(Cell) * term->term_w * (PRINT_H(term) - 1));
                        cells_clear_region(term, 0, 0, PRINT_W(term) - 1, 0);
                    }
                    p->state = ANSI_STATE_NORMAL;
                    break;
                case '7': /* DECSC – save cursor */
                    /* Simple save: store in static vars (single-level save) */
                    /* Extend to a struct if nested saves are needed.         */
                    /* Fall through to normal for now (common extension). */
                    p->state = ANSI_STATE_NORMAL;
                    break;
                case '8': /* DECRC – restore cursor */
                    p->state = ANSI_STATE_NORMAL;
                    break;
                default:
                    /* Unknown two-byte sequence – swallow the introducer. */
                    p->state = ANSI_STATE_NORMAL;
                    break;
            }
            break;

        /* ── Collecting CSI parameters ── */
        case ANSI_STATE_CSI:
            if (c >= '0' && c <= '9') {
                p->cur_param     = p->cur_param * 10 + (c - '0');
                p->has_cur_param = 1;
            } else if (c == ';') {
                ansi_flush_param(p);
            } else if (c >= 0x20 && c <= 0x2F) {
                /* intermediate byte – store one (sufficient for VT-level escapes) */
                p->intermediate = (char)c;
            } else if (c >= 0x40 && c <= 0x7E) {
                /* final byte */
                term_dispatch_csi(term, (char)c);
                p->state = ANSI_STATE_NORMAL;
            } else {
                /* Anything else: malformed; reset */
                p->state = ANSI_STATE_NORMAL;
            }
            break;

        /* ── OSC: skip until ST (ESC \) or BEL ── */
        case ANSI_STATE_OSC:
            if (c == 0x07 /* BEL */ || c == 0x9C /* ST */) {
                p->state = ANSI_STATE_NORMAL;
            } else if (c == 0x1B) {
                /* Next byte should be '\' to form ST; consume it lazily. */
                p->state = ANSI_STATE_ESC;
            }
            break;
    }
}

struct Terminal *term_init(void) {
    struct Terminal *term = malloc(sizeof(struct Terminal));
    memset(term, 0, sizeof(struct Terminal));

    term->term_w   = TERM_W;
    term->term_h   = TERM_H;
    term->char_size = CHAR_SCALE;
    term->attr_fg  = 0xFFFFFF;
    term->attr_bg  = 0x000000;
    term->cursor_x = 0;
    term->cursor_y = 0;

    term->cells = calloc(TERM_W * TERM_H, sizeof(Cell));
    cells_clear_all(term);

    term->ansi.state = ANSI_STATE_NORMAL;

    term->vp_w    = term->term_w * term->char_size;
    term->vp_h    = term->term_h * term->char_size;
    term->bufsize = sizeof(uint32_t) * term->vp_w * term->vp_h;
    term->buf     = memory_requestRegion(term->bufsize);
    term->vp      = vp_open(term->vp_w, term->vp_h, "GWOS Terminal 2");

    vp_add_event_handler(term->vp, term_ev_handler);
    vp_set_buffer(term->vp, term->buf, term->bufsize);
    vp_set_options(term->vp, VP_OPT_RESIZE);
    return term;
}

int main(int argc, char **argv) {
    g_term = term_init();

    int ch_stdout_pipe[2];
    int ch_stdin_pipe[2]; 
    pipe(ch_stdout_pipe);
    pipe(ch_stdin_pipe);

    printf("stdout pipe FDs: read=%d write=%d\n",
           ch_stdout_pipe[0], ch_stdout_pipe[1]);
    printf("stdin  pipe FDs: read=%d write=%d\n",
           ch_stdin_pipe[0],  ch_stdin_pipe[1]);

    struct stat pipe_stat;
    fstat(ch_stdout_pipe[0], &pipe_stat);
    printf("Pipe stat owner: %s\n",  pipe_stat.fs_ownerIden);
    printf("Pipe open stat:  %d\n",  pipe_stat.open_stat);

    EXEC_CTX proc_ctx = exec_createCTX();
    exec_setProgram(proc_ctx, "/A/utils/sh/sh.elf");
    //exec_setProgram(proc_ctx, "/A/lisp/lisp.elf");
    exec_setARGC(proc_ctx, 0);
    exec_setFDCount(proc_ctx, 2);
    /* child's stdin  = read-end of the stdin pipe  */
    exec_setFD(proc_ctx, 0, ch_stdin_pipe[1],  0);
    /* child's stdout = write-end of the stdout pipe */
    exec_setFD(proc_ctx, 1, ch_stdout_pipe[0], 1);
    int child_pid = exec_simple(proc_ctx);
    (void)child_pid;

    char readbuf[64];
    char writebuf[1];
    int  needs_redraw = 0;

    while (1) {
        if (g_term->vp->ascii != '\0') {
            writebuf[0] = g_term->vp->ascii;
            write(ch_stdin_pipe[0], writebuf, 1);
            g_term->vp->ascii = '\0';
        }

        int n = read(ch_stdout_pipe[1], readbuf, sizeof(readbuf));
        if (n > 0) {
            for (int i = 0; i < n; i++)
                term_process_byte(g_term, (unsigned char)readbuf[i]);
            needs_redraw = 1;
        }

        if (needs_redraw && n <= 0) {
            term_draw(g_term);
            needs_redraw = 0;
        }

        if (n <= 0)
            yield();
    }
}