// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_sx6119
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "sx6119.h"
%}

%include "sx6119.h"

