// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_at42qt1070
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "at42qt1070_doc.i"
#endif

%include "at42qt1070.hpp"
%{
    #include "at42qt1070.hpp"
%}
