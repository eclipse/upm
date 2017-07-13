// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_stepmotor
%include "../upm.i"

%include "stdint.i"

%include "stepmotor.hpp"
%{
    #include "stepmotor.hpp"
%}
