%module pyupm_ms5611
%include "../upm.i"

%feature("autodoc", "3");

%include "ms5611.hpp"
%{
    #include "ms5611.hpp"
%}
