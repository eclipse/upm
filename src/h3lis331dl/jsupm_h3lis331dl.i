%module jsupm_h3lis331dl
%include "../upm.i"
%include "cpointer.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%{
    #include "h3lis331dl.h"
%}

%include "h3lis331dl.h"
