%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iPressureSensor.i"
%typemap(javaimports) SWIGTYPE %{import upm_interfaces.*;%}

JAVA_JNI_LOADLIBRARY(javaupm_ms5611)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ms5611.hpp"
%}
%include "ms5611.hpp"
/* END Common SWIG syntax */
