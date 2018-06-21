#ifdef SWIGPYTHON
%module (package="upm") a110x
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

/* Macro for loading javaupm_a110x */
JAVA_JNI_LOADLIBRARY(javaupm_a110x)

/* Macro adding an installisr call w/JAVA runnable to A110X */
JAVA_ADD_INSTALLISR(upm::A110X)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "a110x.hpp"
%}
%include "a110x.hpp"
/* END Common SWIG syntax */
