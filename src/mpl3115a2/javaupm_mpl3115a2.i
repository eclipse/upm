%module javaupm_mpl3115a2
%include "../upm.i"

%{
    #include "mpl3115a2.hpp"
%}

%include "mpl3115a2.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mpl3115a2)