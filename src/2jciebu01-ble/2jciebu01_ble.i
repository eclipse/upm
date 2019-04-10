#ifdef SWIGPYTHON
%module (package="upm") 2jciebu01_ble
#endif

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_2jciebu01_ble)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "2jciebu01_ble.hpp"
#include "2jciebu01.hpp"
%}

%include "2jciebu01_ble.hpp"
%include "2jciebu01.hpp"

/* END Common SWIG syntax */
