// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bh1750
%include "../upm.i"
%include "std_string.i"

%include "bh1750_defs.h"
%include "bh1750.hpp"
%{
    #include "bh1750.hpp"
%}

