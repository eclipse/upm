// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_enc03r
%include "../upm.i"

%include "enc03r.hpp"
%{
    #include "enc03r.hpp"
%}
