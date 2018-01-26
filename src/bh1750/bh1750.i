%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_bh1750)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bh1750_defs.h"
#include "bh1750.hpp"
%}
%include "bh1750_defs.h"
%include "bh1750.hpp"
/* END Common SWIG syntax */
