// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_tcs37727
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "tcs37727_doc.i"
#endif

%include "tcs37727.hpp"
%{
    #include "tcs37727.hpp"
%}
