// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_htu21d
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "htu21d_doc.i"
#endif

%include "htu21d.hpp"
%{
    #include "htu21d.hpp"
%}
