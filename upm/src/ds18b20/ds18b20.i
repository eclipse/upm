%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_ds18b20)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "carrays.i"
%{
#include "ds18b20_defs.h"
#include "ds18b20.hpp"
%}
%include "ds18b20_defs.h"
%include "ds18b20.hpp"
%array_class(char, charArray);
/* END Common SWIG syntax */
