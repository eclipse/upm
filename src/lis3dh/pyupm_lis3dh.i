// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lis3dh
%include "../upm.i"
%include "../upm_vectortypes.i"

#ifdef DOXYGEN
%include "lis3dh_doc.i"
#endif

%include "lis3dh_defs.h"
%include "lis3dh.hpp"
%{
    #include "lis3dh.hpp"
%}
