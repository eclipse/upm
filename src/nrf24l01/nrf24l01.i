%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
#ifndef ANDROID
%module(directors="1") javaupm_nrf24l01
%feature("director") Callback;
SWIG_DIRECTOR_OWNED(Callback)
#endif
%include "arrays_java.i";
%apply signed char[] {uint8_t *};

JAVA_JNI_LOADLIBRARY(javaupm_nrf24l01)
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
#include "nrf24l01.hpp"
%}
%include "Callback.hpp"
%include "nrf24l01.hpp"
/* END Common SWIG syntax */
