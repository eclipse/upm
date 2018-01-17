%module javaupm_vcap
%include "../upm.i"
%include "std_string.i"

%include "vcap.hpp"
%{
    #include "vcap.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_vcap)
