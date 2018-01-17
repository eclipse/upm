%module javaupm_rpr220
%include "../upm.i"


%{
    #include "rpr220.hpp"
%}
%include "rpr220.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_rpr220)
