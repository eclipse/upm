// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ht9170
%include "../upm.i"

%feature("autodoc", "3");

%include "ht9170.hpp"
%{
    #include "ht9170.hpp"
%}
