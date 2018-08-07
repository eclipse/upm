#ifdef SWIGPYTHON
%module (package="upm") lsm303d
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA

%ignore getMagnetometer(float *, float *, float *);
%ignore getAccelerometer(float *, float *, float *);

%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}

JAVA_JNI_LOADLIBRARY(javaupm_lsm303d)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../upm_vectortypes.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../upm_vectortypes.i"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "lsm303d_defs.h"
#include "lsm303d.hpp"
%}
%include "lsm303d_defs.h"
%include "lsm303d.hpp"
/* END Common SWIG syntax */
