// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_emg
%include "../upm.i"

%feature("autodoc", "3");

%include "emg.hpp"
%{
    #include "emg.hpp"
%}
