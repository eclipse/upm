%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"
%typemap(javaimports) SWIGTYPE %{import upm_interfaces.*;%}
%import "../interfaces/javaupm_iCO2Sensor.i"

JAVA_JNI_LOADLIBRARY(javaupm_t6713)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_uint16_t.i"
#endif
/* END Javascript syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "t6713.hpp"
%}
%include "t6713.hpp"
/* END Common SWIG syntax */
