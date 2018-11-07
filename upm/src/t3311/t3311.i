%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_t3311)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(float, floatp);

%{
#include "t3311.hpp"
%}
%include "t3311.hpp"
/* END Common SWIG syntax */
