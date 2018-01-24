%module javaupm_nrf8001
%include "../upm.i"

%{
    #include "nrf8001.hpp"
%}

%include "nrf8001.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_nrf8001)
