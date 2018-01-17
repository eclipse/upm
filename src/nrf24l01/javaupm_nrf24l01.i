%include "../upm.i"

#ifdef ANDROID
    %module javaupm_nrf24l01
#else
    %module(directors="1") javaupm_nrf24l01
    %feature("director") Callback;
    SWIG_DIRECTOR_OWNED(Callback)
#endif


%include "arrays_java.i";
%apply signed char[] {uint8_t *};

%include "Callback.hpp"
%{
    #include "nrf24l01.hpp"
%}

%include "nrf24l01.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_nrf24l01)
