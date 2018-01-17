%module javaupm_ehr
%include "../upm.i"

%ignore beatISR;

%{
    #include "ehr.hpp"
%}

%include "ehr.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ehr)