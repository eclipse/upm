// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ecs1030
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "ecs1030.hpp"
%{
    #include "ecs1030.hpp"
%}
