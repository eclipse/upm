%module javaupm_hcsr04
%include "../upm.i"

%{
    #include "hcsr04.hpp"
%}

%include "hcsr04.h"
%include "hcsr04.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_hcsr04)

