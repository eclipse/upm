// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_uln200xa
%include "../upm.i"

%include "uln200xa_defs.h"
%include "uln200xa.hpp"
%{
    #include "uln200xa.hpp"
%}
