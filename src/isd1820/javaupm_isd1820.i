%module javaupm_isd1820
%include "../upm.i"

%{
    #include "isd1820.hpp"
%}

%include "isd1820.hpp"


JAVA_JNI_LOADLIBRARY(javaupm_isd1820)