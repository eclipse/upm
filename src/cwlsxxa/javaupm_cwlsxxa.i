%module javaupm_cwlsxxa
%include "../upm.i"
%include "std_string.i"

%include "cwlsxxa.hpp"
%{
    #include "cwlsxxa.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_cwlsxxa)
