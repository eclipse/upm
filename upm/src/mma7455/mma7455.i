%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"
%include "std_vector.i"
%apply short *OUTPUT { short * ptrX, short * ptrY, short * ptrZ };
%template(ShortVector) std::vector<short>;

JAVA_JNI_LOADLIBRARY(javaupm_mma7455)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "mma7455.hpp"
%}
%include "mma7455.hpp"
/* END Common SWIG syntax */
