%include "../common_top.i"
%include "iMraa.i"
%include "iLightSensor.i"
%include "iTemperatureSensor.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_noelstemplightreader)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "noelstemplightreader.hpp"
%}
%include "noelstemplightreader.hpp"
/* END Common SWIG syntax */
