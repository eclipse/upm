// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lsm6dsl
%include "../upm.i"
%include "../upm_vectortypes.i"

#ifdef DOXYGEN
%include "lsm6dsl_doc.i"
#endif

%include "lsm6dsl_defs.h"
%include "lsm6dsl.hpp"
%{
    #include "lsm6dsl.hpp"
%}
