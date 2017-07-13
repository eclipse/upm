// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ht9170
%include "../upm.i"

%include "ht9170.hpp"
%{
    #include "ht9170.hpp"
%}
