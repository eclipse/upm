// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ds1307
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "ds1307_doc.i"
#endif

%include "ds1307.hpp"
%{
    #include "ds1307.hpp"
%}
