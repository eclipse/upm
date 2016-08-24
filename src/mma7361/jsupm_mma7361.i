%module jsupm_mma7361
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "mma7361.hpp"
%{
    #include "mma7361.hpp"
%}

