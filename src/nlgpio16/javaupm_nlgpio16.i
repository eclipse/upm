%module javaupm_nlgpio16
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "nlgpio16.hpp"
%}

%include "nlgpio16.hpp"
%array_class(char, charArray);

JAVA_JNI_LOADLIBRARY(javaupm_nlgpio16)
