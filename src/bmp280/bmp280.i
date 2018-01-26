%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iHumiditySensor.i"
%import "../interfaces/javaupm_iPressureSensor.i"

%include "arrays_java.i";
%include "../java_buffer.i"

%typemap(javaimports) SWIGTYPE %{
    import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_bmp280)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "iModuleStatus.hpp"
%include "iTemperatureSensor.hpp"
%include "iPressureSensor.hpp"
%include "iHumiditySensor.hpp"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "iModuleStatus.hpp"
%include "iTemperatureSensor.hpp"
%include "iPressureSensor.hpp"
%include "iHumiditySensor.hpp"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bmp280_regs.h"
#include "bmp280.hpp"
#include "bme280.hpp"
%}
%include "bmp280_regs.h"
%include "bmp280.hpp"
%include "bme280.hpp"
/* END Common SWIG syntax */
