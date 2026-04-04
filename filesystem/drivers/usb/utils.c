#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "utils.h"
/*
 * allocate_aligned
 *
 * Allocates `size` bytes of physically contiguous, alignment-guaranteed
 * memory suitable for use as xHCI data structures (DCBAA, rings, contexts).
 *
 * xHCI requires:
 *   - DCBAA:            64-byte aligned
 *   - Command/Event rings: 64-byte aligned
 *   - Device contexts:  64-byte aligned (or 32-byte if CSZ=0)
 *   - Transfer rings:   16-byte aligned minimum
 *
 * Strategy: allocate (size + align - 1) bytes from the physical memory
 * manager, then round the start address up to the next alignment boundary.
 * The original (unaligned) base pointer is stored just before the returned
 * region so it can be recovered for freeing.
 *
 * Memory layout of the raw allocation:
 *
 *   [ base ][ padding ][ header ][ aligned region ... ]
 *                                ^-- returned pointer
 *
 * The header is one pointer written immediately before the aligned region.
 * It holds `base` so free_aligned() can release the original allocation.
 *
 * @size  - bytes to allocate (must be > 0)
 * @align - alignment in bytes (must be a power of two, minimum 1)
 *
 * Returns a pointer to the aligned region, or NULL on failure.
 */

/* ── Physical memory allocator interface ────────────────────────────────
 *
 * Replace these two functions with your kernel's actual PMM calls.
 * They must return zeroed, physically contiguous memory.
 *
 * pmm_alloc(n)  - allocate n bytes, return virtual address or NULL
 * pmm_free(p,n) - release n bytes previously returned by pmm_alloc
 */
extern void  pmm_free(void *ptr, size_t bytes);

/* ── Internal helpers ───────────────────────────────────────────────── */

static inline bool is_power_of_two(size_t n)
{
    return (n != 0) && ((n & (n - 1)) == 0);
}

/*
 * Round `addr` up to the nearest multiple of `align`.
 * `align` must be a power of two.
 */
static inline uintptr_t align_up(uintptr_t addr, size_t align)
{
    return (addr + (align - 1)) & ~((uintptr_t)(align - 1));
}

/* ── Public API ─────────────────────────────────────────────────────── */

/*
 * Minimum alignment enforced on the header pointer itself.
 * sizeof(void*) is sufficient — 8 bytes on a 64-bit kernel.
 */
#define HEADER_ALIGN sizeof(void *)

void *allocate_aligned(size_t size, size_t align)
{
    if (size == 0 || !is_power_of_two(align))
        return NULL;

    /* Ensure align is at least pointer-sized so the header fits cleanly. */
    if (align < HEADER_ALIGN)
        align = HEADER_ALIGN;

    /*
     * Total raw bytes needed:
     *
     *   align - 1   : worst-case padding to reach the alignment boundary
     *   HEADER_ALIGN: space for the saved base pointer (header)
     *   size        : the caller's actual payload
     *
     * Example — size=128, align=64 on a 64-bit system:
     *   raw = 128 + 64 - 1 + 8 = 199 bytes
     *
     *   If pmm_alloc returns 0x1003 (unaligned):
     *     aligned start of payload region  = align_up(0x1003 + 8, 64)
     *                                      = align_up(0x100B, 64)
     *                                      = 0x1040
     *     header written at               0x1040 - 8 = 0x1038
     *     header holds base pointer       0x1003
     */
    size_t raw = size + align - 1 + HEADER_ALIGN;

    void *base = malloc(raw);
    if (!base)
        return NULL;

    /*
     * Find the aligned address for the payload, leaving room for the
     * header immediately before it.
     */
    uintptr_t base_addr    = (uintptr_t)base;
    uintptr_t after_header = base_addr + HEADER_ALIGN;
    uintptr_t aligned_addr = align_up(after_header, align);

    /*
     * Write the original base pointer into the header slot.
     * The header sits in the padding between `base` and `aligned_addr`,
     * guaranteed to be at least HEADER_ALIGN bytes wide.
     */
    void **header = (void **)(aligned_addr - HEADER_ALIGN);
    *header = base;

    /*
     * Zero the payload region.
     * xHCI structures must be zeroed before use (undefined bits must
     * be 0 per the spec). If pmm_alloc already returns zeroed pages
     * you can remove this, but it is cheap insurance.
     */
    uint8_t *payload = (uint8_t *)aligned_addr;
    for (size_t i = 0; i < size; i++)
        payload[i] = 0;

    return (void *)aligned_addr;
}

/*
 * free_aligned
 *
 * Releases memory returned by allocate_aligned().
 * Recovers the original base pointer from the header and passes it
 * back to the physical memory manager along with the original raw size.
 *
 * @ptr   - pointer previously returned by allocate_aligned()
 * @size  - the same `size` passed to allocate_aligned()
 * @align - the same `align` passed to allocate_aligned()
 */
void free_aligned(void *ptr, size_t size, size_t align)
{
    if (!ptr)
        return;

    if (align < HEADER_ALIGN)
        align = HEADER_ALIGN;

    /* Recover the base pointer from the header slot. */
    void **header = (void **)((uintptr_t)ptr - HEADER_ALIGN);
    void  *base   = *header;

    size_t raw = size + align - 1 + HEADER_ALIGN;
    //pmm_free(base, raw);
}