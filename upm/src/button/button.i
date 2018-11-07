%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%apply int {mraa::Edge}

JAVA_ADD_INSTALLISR_EDGE(upm::Button)

JAVA_JNI_LOADLIBRARY(javaupm_button)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "button.hpp"
%}
%include "button.hpp"
/* END Common SWIG syntax */
