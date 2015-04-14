%module pyupm_waterlevel
%include "../upm.i"

%feature("autodoc", "3");

%include "waterlevel.h"
%{
    #include "waterlevel.h"
%}
