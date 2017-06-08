%include "iLightSensor.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_max44009)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "max44009.hpp"
%}
%include "max44009.hpp"
/* END Common SWIG syntax */
