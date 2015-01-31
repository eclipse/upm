%module jsupm_mma7660
%include "../upm.i"
%include "cpointer.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%{
    #include "mma7660.h"
%}

%include "mma7660.h"
