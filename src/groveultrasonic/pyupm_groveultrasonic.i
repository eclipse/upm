// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveultrasonic
%include "../upm.i"

%include "groveultrasonic.hpp"
%{
    #include "groveultrasonic.hpp"
%}
