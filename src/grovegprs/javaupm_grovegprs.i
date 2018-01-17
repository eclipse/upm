%module javaupm_grovegprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "grovegprs.hpp"
%}

%include "grovegprs.hpp"
%array_class(char, charArray);

JAVA_JNI_LOADLIBRARY(javaupm_grovegprs)