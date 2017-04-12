// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_h803x
%include "../upm.i"
%include "stdint.i"

%feature("autodoc", "3");

%include "h803x.hpp"
%{
    #include "h803x.hpp"
%}
