// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_electromagnet
%include "../upm.i"

%include "electromagnet.hpp"
%{
    #include "electromagnet.hpp"
%}
