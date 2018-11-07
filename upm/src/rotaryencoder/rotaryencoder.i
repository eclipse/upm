%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore signalAISR;

JAVA_JNI_LOADLIBRARY(javaupm_rotaryencoder)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "rotaryencoder.hpp"
%}
%include "rotaryencoder.hpp"
/* END Common SWIG syntax */
