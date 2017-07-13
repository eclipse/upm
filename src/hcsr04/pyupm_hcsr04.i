// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hcsr04
%include "../upm.i"

%include "hcsr04.h"
%include "hcsr04.hpp"
%{
    #include "hcsr04.hpp"
%}
