// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ims
%include "../upm.i"

%{
    #include "ims.hpp"
%}

%include "ims_defs.h"
%include "ims.hpp"
