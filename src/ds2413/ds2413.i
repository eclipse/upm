%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_ds2413)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "carrays.i"
%{
#include "ds2413.hpp"
%}
%include "ds2413.hpp"
%array_class(char, charArray);
/* END Common SWIG syntax */
