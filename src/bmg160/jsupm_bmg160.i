%module jsupm_bmg160
%include "../upm.i"
%include "cpointer.i"
%include "../upm_vectortypes.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "bmg160_defs.h"
%include "bmg160.hpp"
%{
    #include "bmg160.hpp"
%}
