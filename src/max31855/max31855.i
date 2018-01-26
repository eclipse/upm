%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
//! [Interesting]

JAVA_JNI_LOADLIBRARY(javaupm_max31855)
//! [Interesting]
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "max31855.hpp"
%}
%include "max31855.hpp"
/* END Common SWIG syntax */
