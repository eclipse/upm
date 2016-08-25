// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_adafruitms1438
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "adafruitms1438_doc.i"
#endif

%include "adafruitms1438.hpp"
%{
    #include "adafruitms1438.hpp"
%}
