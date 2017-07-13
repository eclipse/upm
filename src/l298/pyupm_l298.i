// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_l298
%include "../upm.i"

%include "l298.hpp"
%{
    #include "l298.hpp"
%}
