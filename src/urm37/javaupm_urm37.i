%module javaupm_urm37
%include "../upm.i"
%include "std_string.i"

%{
    #include "urm37.hpp"
%}

%include "urm37.hpp"


JAVA_JNI_LOADLIBRARY(javaupm_urm37)