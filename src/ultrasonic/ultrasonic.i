%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore signalISR;

JAVA_JNI_LOADLIBRARY(javaupm_ultrasonic)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ultrasonic.hpp"
%}
%include "ultrasonic.hpp"
/* END Common SWIG syntax */
