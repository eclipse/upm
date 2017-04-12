%module pyupm_smartdrive
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"


%include "stdint.i"

%include "smartdrive.hpp"
%{
    #include "smartdrive.hpp"
%}
