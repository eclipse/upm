%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_sht1x)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "sht1x_defs.h"
#include "sht1x.hpp"
%}
%include "sht1x_defs.h"
%include "sht1x.hpp"
/* END Common SWIG syntax */
