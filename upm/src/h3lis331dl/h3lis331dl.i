%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "std_vector.i"

%apply float *OUTPUT { float *aX, float *aY, float *aZ  };
%apply int *OUTPUT { int *x, int *y, int*z };

%ignore i2cContext;

%template(IntVector) std::vector<int>;
%template(FloatVector) std::vector<float>;

JAVA_JNI_LOADLIBRARY(javaupm_h3lis331dl)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%{
#include "h3lis331dl.hpp"
%}
%include "h3lis331dl.hpp"
/* END Common SWIG syntax */
