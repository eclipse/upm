%module javaupm_ds1307
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "ds1307.h"
%}

%include "ds1307.h"
