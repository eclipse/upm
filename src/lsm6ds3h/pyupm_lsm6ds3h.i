// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lsm6ds3h
%include "../upm.i"
%include "../upm_vectortypes.i"

%include "lsm6ds3h_defs.h"
%include "lsm6ds3h.hpp"
%{
    #include "lsm6ds3h.hpp"
%}
