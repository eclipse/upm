%module javaupm_emg
%include "../upm.i"

%{
    #include "emg.hpp"
%}

%include "emg.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_emg)