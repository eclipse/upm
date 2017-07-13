// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovewfs
%include "../upm.i"

%include "grovewfs.hpp"
%{
    #include "grovewfs.hpp"
%}
