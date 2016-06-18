%module pyupm_l3gd20
%include "../upm.i"

%feature("autodoc", "3");

%include "l3gd20.hpp"
%{
    #include "l3gd20.hpp"
%}
