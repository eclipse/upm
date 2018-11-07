%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_rn2903)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "rn2903_defs.h"
#include "rn2903.hpp"
%}
%include "rn2903_defs.h"
%include "rn2903.hpp"
/* END Common SWIG syntax */
