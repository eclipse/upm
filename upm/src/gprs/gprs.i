%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_gprs)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "carrays.i"
%{
#include "gprs.hpp"
%}
%include "gprs.hpp"
%array_class(char, charArray);
/* END Common SWIG syntax */
