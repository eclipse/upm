// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lsm9ds0
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "lsm9ds0.hpp"
%{
    #include "lsm9ds0.hpp"
%}

