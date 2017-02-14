// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_tmp006
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "tmp006_doc.i"
#endif

%include "tmp006.hpp"
%{
    #include "tmp006.hpp"
%}
