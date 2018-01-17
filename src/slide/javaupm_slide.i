%module javaupm_slide
%include "../upm.i"

%{
    #include "slide.hpp"
%}

%include "slide.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_slide)
