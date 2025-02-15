#include "xmath.h"

#define NBITS (48+_DOFF)
#if _DO
#define INIT(w0) 0, 0, 0, w0
#else
#define INIT(w0) w0, 0, 0, 0
#endif

_Dconst _Hugeval = {{INIT(_DMAX<<_DOFF)}};
_Dconst _Inf = {{INIT(_DMAX<<_DOFF)}};
_Dconst _Nan = {{INIT(_DNAN)}};
_Dconst _Rteps = {{(INIT(_DBIAS-NBITS/2) << _DOFF)}};
_Dconst _Xbig = {{INIT((_DBIAS+NBITS/2) << _DOFF)}};


