%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%apply int {mraa::Edge}

JAVA_JNI_LOADLIBRARY(javaupm_led)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "led.hpp"
%}
%include "led.hpp"
/* END Common SWIG syntax */
