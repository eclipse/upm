%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_bmp280)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
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
