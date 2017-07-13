// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mpr121
%include "../upm.i"

#ifdef DOXYGEN
%include "mpr121_doc.i"
#endif

%include "mpr121.hpp"
%{
    #include "mpr121.hpp"
%}
