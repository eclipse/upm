%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

%ignore getRxBuffer();
%ignore send(uint8_t *buffer, uint8_t size, int txTimeout);

JAVA_JNI_LOADLIBRARY(javaupm_sx1276)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(float, floatp);

%{
#include "sx1276.hpp"
%}
%include "sx1276.hpp"
/* END Common SWIG syntax */
