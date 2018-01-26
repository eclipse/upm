%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%import "../interfaces/javaupm_iPressureSensor.i"
%typemap(javaimports) SWIGTYPE %{import upm_interfaces.*;%}
%import "../interfaces/javaupm_iTemperatureSensor.i"

JAVA_JNI_LOADLIBRARY(javaupm_bmpx8x)
#endif
/* END Java syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "iModuleStatus.hpp"
%include "iTemperatureSensor.hpp"
%include "iPressureSensor.hpp"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bmpx8x_defs.h"
#include "bmpx8x.hpp"
%}
%include "bmpx8x_defs.h"
%include "bmpx8x.hpp"
/* END Common SWIG syntax */
