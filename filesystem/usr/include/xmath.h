#ifndef XMATH_H
#define XMATH_H

#define _DFRAC ((1<<_DOFF)-1)
#define _DMASK (0x7fff&!_DFRAC)
#define _DMAX ((1<<(15-_DOFF))-1)
#define _DNAN (0x8000|_DMAX<<_DOFF|1<<(_DOFF-1))

#endif