%module javaupm_t3311
%include "../upm.i"
%include "typemaps.i"
%include "cpointer.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "t3311.hpp"
%}

%include "t3311.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_t3311)
