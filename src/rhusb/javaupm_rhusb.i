%module javaupm_rhusb
%include "../upm.i"
%include "typemaps.i"
%include "cpointer.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "rhusb.hpp"
%}

%include "rhusb.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_rhusb)
