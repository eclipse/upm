%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
#ifndef ANDROID
%module(directors="1") javaupm_rf22
#endif

%include "arrays_java.i"
%apply uint8_t *INOUT { uint8_t* len };
%apply signed char[] {uint8_t*};

JAVA_JNI_LOADLIBRARY(javaupm_rf22)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_uint8_t.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../carrays_uint8_t.i"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "rf22.hpp"
%}
%include "rf22.hpp"
/* END Common SWIG syntax */
