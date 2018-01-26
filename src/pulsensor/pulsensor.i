%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
#ifndef ANDROID
%module(directors="1") javaupm_pulsensor
%feature("director") Callback;
SWIG_DIRECTOR_OWNED(Callback)
#endif
%include "arrays_java.i"

%ignore sample_thread;
%ignore pin_ctx;
%ignore do_sample;
%ignore callback;

JAVA_JNI_LOADLIBRARY(javaupm_pulsensor)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "pulsensor.hpp"
%}
%include "pulsensor.hpp"
%include "Callback.hpp"
/* END Common SWIG syntax */
