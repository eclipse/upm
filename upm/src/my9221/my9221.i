%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_my9221)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "groveledbar.hpp"
#include "my9221.hpp"
#include "grovecircularled.hpp"
%}
%include "my9221.hpp"
%include "groveledbar.hpp"
%include "grovecircularled.hpp"
/* END Common SWIG syntax */
