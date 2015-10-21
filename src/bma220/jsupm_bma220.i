%module jsupm_bma220
%include "../upm.i"
%include "cpointer.i"

%pointer_functions(float, floatp);

%include "bma220.h"
%{
    #include "bma220.h"
%}

