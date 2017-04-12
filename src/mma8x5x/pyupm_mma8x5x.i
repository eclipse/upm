// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mma8x5x
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "mma8x5x_doc.i"
#endif

%include "mma8x5x.hpp"
%{
    #include "mma8x5x.hpp"
%}
