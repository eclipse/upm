%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_xbee)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "carrays.i"
%{
#include "xbee.hpp"
%}
%include "xbee.hpp"
%array_class(char, charArray);
/* END Common SWIG syntax */
