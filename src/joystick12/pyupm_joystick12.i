// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_joystick12
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "joystick12.h"
%{
    #include "joystick12.h"
%}
