// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_md
%include "../upm.i"

%include "md_defs.h"
%include "md.hpp"
%{
    #include "md.hpp"
%}
