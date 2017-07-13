// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ttp223
%include "../upm.i"

%include "ttp223.hpp"
%{
    #include "ttp223.hpp"
%}

