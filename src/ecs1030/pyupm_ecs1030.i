%module pyupm_ecs1030
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "ecs1030.h"
%{
    #include "ecs1030.h"
%}
