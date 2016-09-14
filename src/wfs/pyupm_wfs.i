// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_wfs
%include "../upm.i"

%feature("autodoc", "3");

%include "wfs.hpp"
%{
    #include "wfs.hpp"
%}
