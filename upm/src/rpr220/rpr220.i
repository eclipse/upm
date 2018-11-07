%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_ADD_INSTALLISR(upm::RPR220)
JAVA_JNI_LOADLIBRARY(javaupm_rpr220)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "rpr220.hpp"
%}
%include "rpr220.hpp"
/* END Common SWIG syntax */
