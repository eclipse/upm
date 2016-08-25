// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hp20x
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "hp20x_doc.i"
#endif

%include "hp20x.hpp"
%{
    #include "hp20x.hpp"
%}
