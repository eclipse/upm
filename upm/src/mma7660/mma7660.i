%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"
%include "std_vector.i"

%apply float *OUTPUT { float *ax, float *ay, float *az  };
%apply int *OUTPUT { int *x, int *y, int *z };
%ignore installISR(int , void *, void *);

%template(IntVector) std::vector<int>;
%template(FloatVector) std::vector<float>;

JAVA_ADD_INSTALLISR_PIN(upm::MMA7660)
JAVA_JNI_LOADLIBRARY(javaupm_mma7660)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%{
#include "mma7660_regs.h"
#include "mma7660.hpp"
%}
%include "mma7660_regs.h"
%include "mma7660.hpp"
/* END Common SWIG syntax */
