%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_ads1x15)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ads1x15.hpp"
#include "ads1015.hpp"
#include "ads1115.hpp"
%}
%include "ads1x15.hpp"
%include "ads1115.hpp"
%include "ads1015.hpp"
/* END Common SWIG syntax */
