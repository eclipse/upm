// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mpl3115a2
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "mpl3115a2_doc.i"
#endif

%include "mpl3115a2.hpp"
%{
    #include "mpl3115a2.hpp"
%}
