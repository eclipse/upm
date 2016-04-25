// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grove
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "grove_doc.i"
#endif

%include "grove.hpp"
%{
    #include "grove.hpp"
%}
