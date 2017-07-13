// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lcdks
%include "../upm.i"
%include "../upm_vectortypes.i"

%include "lcdks.hpp"
%{
    #include "lcdks.hpp"
%}
