// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bma220
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "bma220.hpp"
%{
    #include "bma220.hpp"
%}

