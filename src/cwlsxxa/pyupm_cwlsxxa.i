// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_cwlsxxa
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "cwlsxxa.hpp"
%}
%include "cwlsxxa.hpp"

