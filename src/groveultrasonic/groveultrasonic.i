%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore signalISR;

JAVA_JNI_LOADLIBRARY(javaupm_groveultrasonic)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "groveultrasonic.hpp"
%}
%include "groveultrasonic.hpp"
/* END Common SWIG syntax */
