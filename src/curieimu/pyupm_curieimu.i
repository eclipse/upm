// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_curieimu
%include "../upm.i"

%include "stdint.i"

%include "curieimu.hpp"
%{
    #include "curieimu.hpp"
%}
