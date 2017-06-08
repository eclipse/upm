%include "../common_top.i"
%include "iLightController.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_ds1808lc)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ds1808lc.hpp"
%}
%include "ds1808lc.hpp"
/* END Common SWIG syntax */
