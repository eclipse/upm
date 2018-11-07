%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

%apply int *OUTPUT { int *gas, int *temp };

JAVA_JNI_LOADLIBRARY(javaupm_mhz16)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(int, intp);

%{
#include "mhz16.hpp"
speed_t int_B9600 = B9600;
%}
%include "mhz16.hpp"
speed_t int_B9600 = B9600;
/* END Common SWIG syntax */
