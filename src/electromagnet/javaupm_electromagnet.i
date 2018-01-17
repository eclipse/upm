%module javaupm_electromagnet
%include "../upm.i"

%{
    #include "electromagnet.hpp"
%}

%include "electromagnet.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_electromagnet)