// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_enc03r
%include "../upm.i"

%feature("autodoc", "3");

%include "enc03r.h"
%{
    #include "enc03r.h"
%}
