%module jsupm_bmi160
%include "../upm.i"
%include "cpointer.i"

%pointer_functions(float, floatp);

%include "bmi160.hpp"
%{
    #include "bmi160.hpp"
%}
