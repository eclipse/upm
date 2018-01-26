%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"
%typemap(javaimports) SWIGTYPE %{import upm_interfaces.*;%}

%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iHumiditySensor.i"

JAVA_JNI_LOADLIBRARY(javaupm_si7005)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "si7005.hpp"
%}
%include "si7005.hpp"
/* END Common SWIG syntax */
