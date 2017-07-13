// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_tca9548a
%include "../upm.i"

%include "tca9548a.hpp"
%{
    #include "tca9548a.hpp"
%}
