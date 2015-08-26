%module jsupm_mhz16
%include "../upm.i"
%include "cpointer.i"

/* Send "int *" to JavaScript as intp */
%pointer_functions(int, intp);
%{
    #include "mhz16.h"
    speed_t int_B9600 = B9600;
%}

%include "mhz16.h"
speed_t int_B9600 = B9600;
