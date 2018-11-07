%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore font;
%ignore m_map;

JAVA_JNI_LOADLIBRARY(javaupm_ssd1351)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ssd1351.hpp"
#include "ssd1351_gfx.hpp"
%}
%include "ssd1351_gfx.hpp"
%include "ssd1351.hpp"
/* END Common SWIG syntax */
