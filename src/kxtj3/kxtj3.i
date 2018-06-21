%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_kxtj3)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "std_vector.i"
%template(floatVector) std::vector<float>;

%apply float *OUTPUT {float *x, float *y, float *z};

%{
#include "kxtj3.hpp"
%}
%include "kxtj3.h"
%include "kxtj3.hpp"
/* END Common SWIG syntax */
