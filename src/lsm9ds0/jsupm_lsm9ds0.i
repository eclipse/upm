%module jsupm_lsm9ds0
%include "../upm.i"
%include "cpointer.i"

%pointer_functions(float, floatp);

%include "lsm9ds0.h"
%{
    #include "lsm9ds0.h"
%}

