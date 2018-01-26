%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%apply uint8_t *INPUT { uint8_t *addr }

JAVA_JNI_LOADLIBRARY(javaupm_ili9341)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ili9341_gfx.hpp"
#include "ili9341.hpp"
%}
%include "ili9341_gfx.hpp"
%include "ili9341.hpp"
/* END Common SWIG syntax */
