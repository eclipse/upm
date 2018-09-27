#ifdef SWIGPYTHON
%module (package="upm") mpu9150
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "std_vector.i"
%include "arrays_java.i"
%include "../java_buffer.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}

%apply int {mraa::Edge};

%define GETTER get_gpioIRQ()
%enddef

JAVA_ADD_INSTALLISR_GPIO(upm::MPU60X0)

JAVA_JNI_LOADLIBRARY(javaupm_mpu9150)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "std_vector.i"
%include "../upm_vectortypes.i"
%pointer_functions(float, floatp);
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "std_vector.i"
%include "../upm_vectortypes.i"
%pointer_functions(float, floatp);
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ak8975.hpp"
#include "mpu60x0.hpp"
#include "mpu9150.hpp"
#include "mpu9250.hpp"
%}
%include "ak8975.hpp"
%include "mpu60x0.hpp"
%include "mpu9150.hpp"
%include "mpu9250.hpp"
/* END Common SWIG syntax */
