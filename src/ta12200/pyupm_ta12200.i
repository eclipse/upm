// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ta12200
%include "../upm.i"

%feature("autodoc", "3");

%include "ta12200.hpp"
%{
    #include "ta12200.hpp"
%}
