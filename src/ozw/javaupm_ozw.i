%module javaupm_ozw
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "ozw.h"
%}

%include "ozw.h"
