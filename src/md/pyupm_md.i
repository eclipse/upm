// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_md
%include "../upm.i"

#ifdef DOXYGEN
%include "md_doc.i"
#endif

%include "md_defs.h"
%include "md.hpp"
%{
    #include "md.hpp"
%}
