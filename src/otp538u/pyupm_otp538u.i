// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_otp538u
%include "../upm.i"

%feature("autodoc", "3");

%include "otp538u.h"
%{
    #include "otp538u.h"
%}
