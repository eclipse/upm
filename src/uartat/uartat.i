%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_uartat)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "uartat_defs.h"
#include "uartat.hpp"
%}
%include "uartat_defs.h"
%include "uartat.hpp"
/* END Common SWIG syntax */
