// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_tsl2561
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "tsl2561_doc.i"
#endif

%include "tsl2561.hpp"
%{
    #include "tsl2561.hpp"
%}
