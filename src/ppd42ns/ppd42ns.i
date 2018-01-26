%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_ppd42ns)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ppd42ns.hpp"
%}
%include "ppd42ns_data.h"
%include "ppd42ns.hpp"
/* END Common SWIG syntax */
