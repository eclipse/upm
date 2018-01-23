%module javaupm_ds1307
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "ds1307.hpp"
%}

%include "ds1307.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ds1307)