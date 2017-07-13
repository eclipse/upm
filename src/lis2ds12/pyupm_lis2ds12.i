// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lis2ds12
%include "../upm.i"
%include "../upm_vectortypes.i"

#ifdef DOXYGEN
%include "lis2ds12_doc.i"
#endif

%include "lis2ds12_defs.h"
%include "lis2ds12.hpp"
%{
    #include "lis2ds12.hpp"
%}
