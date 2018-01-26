%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_hcsr04)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "hcsr04.hpp"
%}
%include "hcsr04.h"
%include "hcsr04.hpp"
/* END Common SWIG syntax */
