%module javaupm_button
%include "../upm.i"

%apply int {mraa::Edge}

%{
    #include "button.hpp"
%}

%include "button.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_button)
