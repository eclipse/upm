%module javaupm_rgbringcoder
%include "../upm.i"

%{
    #include "rgbringcoder.hpp"
%}

%include "rgbringcoder.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_rgbringcoder)