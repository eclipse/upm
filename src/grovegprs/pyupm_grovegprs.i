// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovegprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "grovegprs.h"
%}
%include "grovegprs.h"
%array_class(char, charArray);
