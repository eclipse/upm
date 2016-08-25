// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_am2315
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "am2315_doc.i"
#endif

%include "am2315.hpp"
%{
    #include "am2315.hpp"
%}
