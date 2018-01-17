%module javaupm_grovemoisture
%include "../upm.i"

%{
    #include "grovemoisture.hpp"
%}

%include "grovemoisture.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovemoisture)