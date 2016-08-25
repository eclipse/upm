// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_max5487
%include "../upm.i"

%feature("autodoc", "3");

%include "max5487.hpp"
%{
    #include "max5487.hpp"
%}
