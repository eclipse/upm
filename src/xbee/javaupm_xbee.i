%module javaupm_xbee
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "xbee.hpp"
%}

%include "xbee.hpp"
%array_class(char, charArray);

JAVA_JNI_LOADLIBRARY(javaupm_xbee)