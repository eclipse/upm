%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore getHumidityData(float*, float*, float*);

JAVA_JNI_LOADLIBRARY(javaupm_htu21d)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "htu21d.hpp"
%}
%include "htu21d.hpp"
/* END Common SWIG syntax */
