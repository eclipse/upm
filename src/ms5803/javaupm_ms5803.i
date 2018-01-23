%module javaupm_ms5803
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "../java_buffer.i"
%include "std_string.i"

%include "ms5803_defs.h"
%include "ms5803.hpp"
%{
    #include "ms5803.hpp"
%}


JAVA_JNI_LOADLIBRARY(javaupm_ms5803)
