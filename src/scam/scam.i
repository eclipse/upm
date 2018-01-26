%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_scam)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "scam.hpp"
%}
%include "scam.hpp"
/* END Common SWIG syntax */
