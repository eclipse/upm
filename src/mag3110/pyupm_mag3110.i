// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mag3110
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "mag3110_doc.i"
#endif

%include "mag3110.hpp"
%{
    #include "mag3110.hpp"
%}
