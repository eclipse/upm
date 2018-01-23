#ifdef ANDROID
    %module javaupm_max30100
#else
    %module(directors="1", threads="1") javaupm_max30100
#endif

%include "../upm.i"

%{
    #include "max30100.hpp"
%}

#ifndef ANDROID
    %feature("director") upm::Callback;
#endif

%include "max30100_regs.h"
%include "max30100.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_max30100)
