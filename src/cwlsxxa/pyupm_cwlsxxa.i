// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_cwlsxxa
%include "../upm.i"
%include "std_string.i"

%{
    #include "cwlsxxa.hpp"
%}
%include "cwlsxxa.hpp"

