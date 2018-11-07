%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_tm1637)
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
%rename("writeArray")  write(uint8_t *digits);
%rename("writeString") write(std::string digits);

%varargs(4, int digit = 0) write;
%apply uint8_t *INPUT { uint8_t *digits }

%{
#include "tm1637.hpp"
%}
%include "tm1637.hpp"
/* END Common SWIG syntax */
