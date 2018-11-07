%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_t8100)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bacnetmstp.hpp"
#include "bacnetutil.hpp"
#include "t8100.hpp"
%}
%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"
%include "t8100.hpp"
/* END Common SWIG syntax */
