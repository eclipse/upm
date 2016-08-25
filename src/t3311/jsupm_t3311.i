%module jsupm_t3311
%include "../upm.i"
%include "stdint.i"
%include "cpointer.i"

%pointer_functions(float, floatp);

%include "t3311.hpp"
%{
    #include "t3311.hpp"
%}
