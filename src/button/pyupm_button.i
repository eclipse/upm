// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_button
%include "../upm.i"

%include "button.hpp"
%{
    #include "button.hpp"
%}
