%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_bmpx8x)
#endif
/* END Java syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bmpx8x_defs.h"
#include "bmpx8x.hpp"
%}
%include "bmpx8x_defs.h"
%include "bmpx8x.hpp"
/* END Common SWIG syntax */
