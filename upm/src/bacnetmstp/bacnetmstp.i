%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../carrays_uint32_t.i"
JAVA_JNI_LOADLIBRARY(javaupm_bacnetmstp)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(float, floatp);
%{
#include "bacnetmstp.hpp"
%}
%include "bacnetmstp.hpp"
/* END Common SWIG syntax */
