%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore flowISR;

JAVA_JNI_LOADLIBRARY(javaupm_grovewfs)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "grovewfs.hpp"
%}
%include "grovewfs.hpp"
/* END Common SWIG syntax */
