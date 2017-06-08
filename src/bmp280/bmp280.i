%include "../common_top.i"
%include "iHumiditySensor.i"
%include "iTemperatureSensor.i"
%include "iPressureSensor.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_bmp280)
#endif
/* END Java syntax */

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
