// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_initioSampleModule
%include "../upm.i"

%include "initioSampleModule.hpp"
%{
    #include "initioSampleModule.hpp"
%}
