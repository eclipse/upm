#ifdef SWIGPYTHON
%module (package="upm") teams
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_teams)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "teams.hpp"
%}
%include "teams.hpp"
/* END Common SWIG syntax */
