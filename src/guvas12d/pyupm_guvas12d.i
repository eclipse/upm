// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_guvas12d
%include "../upm.i"

%include "guvas12d.hpp"
%{
    #include "guvas12d.hpp"
%}
