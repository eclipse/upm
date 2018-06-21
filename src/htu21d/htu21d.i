#ifdef SWIGPYTHON
%module (package="pyupm_new_interfaces") htu21d
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

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
