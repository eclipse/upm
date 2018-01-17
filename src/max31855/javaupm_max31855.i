//! [Interesting]
%module javaupm_max31855
%include "../upm.i"

%{
    #include "max31855.hpp"
%}

%include "max31855.hpp"
//! [Interesting]

JAVA_JNI_LOADLIBRARY(javaupm_max31855)