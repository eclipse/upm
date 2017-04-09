// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_collision
%include "../upm.i"

%feature("autodoc", "3");

%include "collision.hpp"
%{
    #include "collision.hpp"
%}
