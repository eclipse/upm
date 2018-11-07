%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i"
%ignore i2cContext;
%apply signed char[] {uint8_t *};
%apply unsigned char *OUTPUT {uint8_t *uidLength};
%apply unsigned char *INOUT {uint8_t *responseLength};

JAVA_JNI_LOADLIBRARY(javaupm_pn532)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_uint8_t.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../carrays_uint8_t.i"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "pn532.hpp"
%}
%include "pn532.hpp"
/* END Common SWIG syntax */
