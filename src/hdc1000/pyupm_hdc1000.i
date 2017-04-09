// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hdc1000
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "hdc1000_doc.i"
#endif

%include "hdc1000.hpp"
%{
    #include "hdc1000.hpp"
%}
