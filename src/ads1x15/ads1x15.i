%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%import "../interfaces/javaupm_iADC.i"
%typemap(javaimports) SWIGTYPE %{import upm_interfaces.*;%}

JAVA_JNI_LOADLIBRARY(javaupm_ads1x15)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "iModuleStatus.hpp"
%include "iADC.hpp"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "iModuleStatus.hpp"
%include "iADC.hpp"
#endif
/* END Python syntax */

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
