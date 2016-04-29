%module jsupm_bmp280
%include "../upm.i"
%include "cpointer.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "bmp280.hpp"
%{
    #include "bmp280.hpp"
%}
