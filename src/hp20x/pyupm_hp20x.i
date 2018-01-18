// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hp20x
%include "../upm.i"

%include "hp20x.hpp"
%{
    #include "hp20x.hpp"
%}
