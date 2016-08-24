// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bh1750
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "bh1750.hpp"
%}
%include "bh1750.hpp"

