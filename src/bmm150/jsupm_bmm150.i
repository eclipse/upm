%module jsupm_bmm150
%include "../upm.i"
%include "cpointer.i"
%include "../upm_vectortypes.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "bmm150_defs.h"
%include "bmm150.hpp"
%{
    #include "bmm150.hpp"
%}
