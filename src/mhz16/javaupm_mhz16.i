%module javaupm_mhz16
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%apply int *OUTPUT { int *gas, int *temp };

%{
    #include "mhz16.h"
    speed_t int_B9600 = B9600;
%}

%include "mhz16.h"
speed_t int_B9600 = B9600;
