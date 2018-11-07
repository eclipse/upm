%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_nlgpio16)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "carrays.i"
%{
#include "nlgpio16.hpp"
%}
%include "nlgpio16.hpp"
%array_class(char, charArray);
/* END Common SWIG syntax */
