%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_md)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "md_defs.h"
#include "md.hpp"
%}
%include "md_defs.h"
%include "md.hpp"
/* END Common SWIG syntax */
