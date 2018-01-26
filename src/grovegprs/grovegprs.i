%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_grovegprs)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "carrays.i"
%{
#include "grovegprs.hpp"
%}
%include "grovegprs.hpp"
%array_class(char, charArray);
/* END Common SWIG syntax */
