%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i";

JAVA_JNI_LOADLIBRARY(javaupm_zfm20)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%apply uint16_t &OUTPUT {uint16_t &id};
%apply uint16_t &OUTPUT {uint16_t &score};
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"
%pointer_functions(int, intp);

%{
#include "zfm20.hpp"
%}
%include "zfm20.hpp"
/* END Common SWIG syntax */
