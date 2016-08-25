// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_buzzer
%include "../upm.i"

%feature("autodoc", "3");

%include "buzzer.hpp"
%{
    #include "buzzer.hpp"
%}
