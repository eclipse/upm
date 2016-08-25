// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_maxsonarez
%include "../upm.i"

%feature("autodoc", "3");

%include "maxsonarez.hpp"
%{
    #include "maxsonarez.hpp"
%}
