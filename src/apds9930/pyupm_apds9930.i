%module pyupm_apds9930
%include "../upm.i"

%feature("autodoc", "3");

%include "apds9930.hpp"
%{
    #include "apds9930.hpp"
%}
