%module javaupm_apds9930
%include "../upm.i"

%{
    #include "apds9930.hpp"
%}
%include "apds9930.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_apds9930)
