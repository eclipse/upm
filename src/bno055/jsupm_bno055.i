%module jsupm_bno055
%include "../upm.i"
%include "cpointer.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "bno055.hpp"
%{
    #include "bno055.hpp"
%}
