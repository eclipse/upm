%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_grovescam)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "grovescam.hpp"
%}
%include "grovescam.hpp"
/* END Common SWIG syntax */
