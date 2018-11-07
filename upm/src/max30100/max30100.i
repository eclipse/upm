%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
#ifndef ANDROID
%module(directors="1", threads="1") javaupm_max30100
%feature("director") upm::Callback;
#endif
JAVA_JNI_LOADLIBRARY(javaupm_max30100)
#endif
/* END Java syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%module(directors="1", threads="1") pyupm_max30100

%feature("director") upm::Callback;
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "max30100_regs.h"
#include "max30100.hpp"
%}
%include "max30100_regs.h"
%include "max30100.hpp"
/* END Common SWIG syntax */
