// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ppd42ns
%include "../upm.i"

%feature("autodoc", "3");

%include "ppd42ns.hpp"
%{
    #include "ppd42ns.hpp"
%}
