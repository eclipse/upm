%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_nunchuck)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "nunchuck.hpp"
%}
%include "nunchuck.hpp"
/* END Common SWIG syntax */
