%module jsupm_dfrec
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "dfrec.hpp"
%{
    #include "dfrec.hpp"
%}
