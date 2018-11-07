%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_dfrorp)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%{
#include "dfrorp.hpp"
%}
%include "dfrorp.hpp"
/* END Common SWIG syntax */
