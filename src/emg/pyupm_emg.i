// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_emg
%include "../upm.i"

%include "emg.hpp"
%{
    #include "emg.hpp"
%}
