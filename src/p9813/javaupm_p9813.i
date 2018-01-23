%module javaupm_p9813

%include "../upm.i"
%include "typemaps.i"

%{
    #include "p9813.hpp"
%}
%include "p9813.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_p9813)
