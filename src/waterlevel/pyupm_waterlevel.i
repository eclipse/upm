// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_waterlevel
%include "../upm.i"

%feature("autodoc", "3");

%include "waterlevel.hpp"
%{
    #include "waterlevel.hpp"
%}
