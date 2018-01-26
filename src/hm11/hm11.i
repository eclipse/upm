%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_hm11)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "carrays.i"

%{
#include "hm11.hpp"
speed_t int_B9600 = B9600;
%}
%include "hm11.hpp"

%array_class(char, charArray);

speed_t int_B9600 = B9600;
/* END Common SWIG syntax */
