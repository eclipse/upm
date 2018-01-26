%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_ecezo)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ecezo_defs.h"
#include "ecezo.hpp"
%}
%include "ecezo_defs.h"
%include "ecezo.hpp"
/* END Common SWIG syntax */
