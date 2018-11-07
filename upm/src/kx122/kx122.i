%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_kx122)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "std_vector.i"
%template(floatVector) std::vector<float>;

%apply float *OUTPUT {float *x, float *y, float *z};

%{
#include "kx122.hpp"
%}
%include "kx122.h"
%include "kx122.hpp"
/* END Common SWIG syntax */
