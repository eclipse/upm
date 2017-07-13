// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_led
%include "../upm.i"

%include "led.hpp"
%{
    #include "led.hpp"
%}
