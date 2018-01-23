#ifdef ANDROID
    %module javaupm_rf22
#else
    %module(directors="1") javaupm_rf22
#endif

%include "../upm.i"
%include "arrays_java.i"
%include "typemaps.i"

%apply uint8_t *INOUT { uint8_t* len };
%apply signed char[] {uint8_t*};

%{
    #include "rf22.hpp"
%}

%include "rf22.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_rf22)
