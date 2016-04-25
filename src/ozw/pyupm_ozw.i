// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ozw
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "ozw.hpp"
%{
    #include "ozw.hpp"
%}

