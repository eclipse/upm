%module javaupm_groveehr
%include "../upm.i"

%ignore beatISR;

%{
    #include "groveehr.h"
%}

%include "groveehr.h"
