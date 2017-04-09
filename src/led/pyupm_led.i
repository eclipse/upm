// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_led
%include "../upm.i"

%feature("autodoc", "3");

%include "led.hpp"
%{
    #include "led.hpp"
%}
