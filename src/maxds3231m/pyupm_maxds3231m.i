// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_maxds3231m
%include "../upm.i"

%feature("autodoc", "3");

%include "maxds3231m.hpp"
%{
    #include "maxds3231m.hpp"
%}
