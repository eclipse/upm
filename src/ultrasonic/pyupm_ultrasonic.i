// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ultrasonic
%include "../upm.i"

%feature("autodoc", "3");

%include "ultrasonic.hpp"
%{
    #include "ultrasonic.hpp"
%}
