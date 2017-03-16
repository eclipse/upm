// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_jhd1313m1
%include "../upm.i"
%include "../upm_vectortypes.i"

%feature("autodoc", "3");

%include "jhd1313m1.hpp"
%{
    #include "jhd1313m1.hpp"
%}
