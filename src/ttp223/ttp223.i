#ifdef SWIGPYTHON
%module (package="upm") ttp223
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%module (docstring="TTP223 Touch Sensor") javaupm_ttp223
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}


%apply int {mraa::Edge}

JAVA_ADD_INSTALLISR_EDGE(upm::TTP223)

JAVA_JNI_LOADLIBRARY(javaupm_ttp223)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ttp223.hpp"
%}
%include "ttp223.hpp"
/* END Common SWIG syntax */
