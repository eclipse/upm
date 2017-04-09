%module jsupm_ms5803
%include "../upm.i"
%include "cpointer.i"
%include "std_string.i"

%pointer_functions(float, floatp);

%include "ms5803_defs.h"
%include "ms5803.hpp"
%{
    #include "ms5803.hpp"
%}
