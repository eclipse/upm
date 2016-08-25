%module pyupm_max44009
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "max44009.hpp"
%{
    #include "max44009.hpp"
%}
