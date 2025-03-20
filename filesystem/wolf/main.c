#include <stdio.h>
#include <string.h>
#include <sys/window.h>
#include <sys/task.h>

#define PI 3.1415
#define PI2 6.283

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    int x;
    int y;
} Vector2Int;

#define dot(v0, v1)                  \
    ({ const Vector2 _v0 = (v0), _v1 = (v1); (_v0.x * _v1.x) + (_v0.y * _v1.y); })
#define length(v) ({ const Vector2 _v = (v); sqrtf(dot(_v, _v)); })
#define normalize(u) ({              \
        const Vector2 _u = (u);           \
        const float l = length(_u);    \
        (Vector2) { _u.x / l, _u.y / l }; \
    })
#define min(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a < _b ? _a : _b; })
#define max(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a > _b ? _a : _b; })
#define sign(a) ({                                       \
    __typeof__(a) _a = (a);                          \
    (__typeof__(a))(_a < 0 ? -1 : (_a > 0 ? 1 : 0)); \
})

struct WINDOW *window;
uint32_t *win_buf;



typedef struct {
    Vector2 position;
    Vector2 direction;
    Vector2 plane;
} Player;

Player player;

#define MAP_SIZE 10
uint8_t Map[MAP_SIZE * MAP_SIZE] = {
    1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,0,3,3,3,0,0,0,1,
    1,0,0,2,0,0,0,0,0,1,
    1,0,0,2,0,1,0,0,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,
};

uint32_t colors[] = {
    0x0,
    0x00FF0000,
    0x0000FF00,
    0x000000FF
};

void render();
void drawVertLine(int x, int y1, int y2, uint32_t color);
float fabsf(float x);
float sqrtf(float x);
float cos(float x);
float sin(float x);
void rotate(float rot);

int main(int argc, char **argv){
    puts("Opening Wolfenstein Style Renderer");
    window = window_open("MapRender", 1);
    win_buf = window->backbuffer;
    memset(win_buf, 0, window->width * window->height * sizeof(uint32_t));
    set_schedule(ONFOCUS);

    player.position = (Vector2) {3,3};
    player.direction = normalize(((Vector2) { -1.0f, 0.1f }));
    player.plane = (Vector2) {0.0f, 0.66f};

    
    float rotspeed = 3.0f * 0.016f;
    float movespeed = 3.0f * 0.016f;

    while(1){
        //memset(win_buf, 0, window->width * window->height * sizeof(uint32_t));
        render();
        window_update();

        char c = window_getc();
        switch(c){
            case 'a':
                rotate(+rotspeed);
                break;
            case 'd':
                rotate(-rotspeed);
                break;
            case 'w':
                player.position.x += player.direction.x * movespeed;
                player.position.y += player.direction.y * movespeed;
                break;
            case 's':
                player.position.x -= player.direction.x * movespeed;
                player.position.y -= player.direction.y * movespeed;
                break;
        }
    }
}

void rotate(float rot) {
    const Vector2 d = player.direction, p = player.plane;
    player.direction.x = d.x * cos(rot) - d.y * sin(rot);
    player.direction.y = d.x * sin(rot) + d.y * cos(rot);
    player.plane.x = p.x * cos(rot) - p.y * sin(rot);
    player.plane.y = p.x * sin(rot) + p.y * cos(rot);
}

void render(){
    for(int x = 0; x < window->width; x++){
        float xcam = (2 * (x / (float) (window->width))) - 1;
        Vector2 dir = {
            player.direction.x + player.plane.x * xcam,
            player.direction.y + player.plane.y * xcam
        };

        Vector2 pos = player.position;
        Vector2Int intPos = {
            (int) pos.x,
            (int) pos.y
        };

        Vector2 deltadist = {
            fabsf(dir.x) < 1e-20 ? 1e30 : fabsf(1.0f / dir.x),
            fabsf(dir.y) < 1e-20 ? 1e30 : fabsf(1.0f / dir.y),
        };

        Vector2 sidedist = {
            deltadist.x * (dir.x < 0 ? (pos.x - intPos.x) : (intPos.x + 1 - pos.x)),
            deltadist.y * (dir.y < 0 ? (pos.y - intPos.y) : (intPos.y + 1 - pos.y)),
        };

        Vector2Int step = { (int) sign(dir.x), (int) sign(dir.y) };

        struct { int val, side; Vector2 pos; } hit = { 0, 0, { 0.0f, 0.0f } };

        while (!hit.val) {
            if (sidedist.x < sidedist.y) {
                sidedist.x += deltadist.x;
                intPos.x += step.x;
                hit.side = 0;
            } else {
                sidedist.y += deltadist.y;
                intPos.y += step.y;
                hit.side = 1;
            }

            hit.val = Map[intPos.y * MAP_SIZE + intPos.x];
        }

        uint32_t color = colors[hit.val];
        if (hit.side == 1) {
            const uint32_t
                br = ((color & 0xFF00FF) * 0xC0) >> 8,
                g  = ((color & 0x00FF00) * 0xC0) >> 8;

            color = 0xFF000000 | (br & 0xFF00FF) | (g & 0x00FF00);
        }

        hit.pos = (Vector2) { pos.x + sidedist.x, pos.y + sidedist.y };

        const float dperp =
            hit.side == 0 ?
                (sidedist.x - deltadist.x)
                : (sidedist.y - deltadist.y);

        
        const int h = (int) (window->height / dperp);
        const int y0 = max((window->height / 2) - (h / 2), 0);
        const int y1 = min((window->height / 2) + (h / 2), window->height - 1);

        drawVertLine(x, 0, y0, 0xFF202020);
        drawVertLine(x, y0, y1, color);
        drawVertLine(x, y1, window->height - 1, 0xFF505050);
    }
}

void drawVertLine(int x, int y1, int y2, uint32_t color){
    //if(y1 > window->height || y1 < 0 || y2 > window->height || y2 < 0 || x < 0 || x > window->width) return;
    if(y1 < 0) y1 = 0;
    if(y2 > window->height) y2 = window->height;
    
    for(int y = y1; y < y2; y++){
        win_buf[x + window->width * y] = color;
    }
}

float fabsf(float x){
    if(x < 0) return -1 * x;
    return x;
}

float sqrtf(float x) {
  union {
    int i;
    float x;
  } u;
  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1 << 22);

  u.x = u.x + x/u.x;
  u.x = 0.25f*u.x + x/u.x;

  return u.x;
}

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float cos(float x)
{
  x = modd(x, PI2);
  return 1 - ((x * x) / (2)) + ((x * x * x * x) / (24)) - ((x * x * x * x * x * x) / (720)) + ((x * x * x * x * x * x * x * x) / (40320)) - ((x * x * x * x * x * x * x * x * x * x) / (3628800)) + ((x * x * x * x * x * x * x * x * x * x * x * x) / (479001600));
}

float sin(float x){
  return cos(x - (PI / 2));
}