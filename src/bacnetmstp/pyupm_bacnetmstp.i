// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bacnetmstp
%include "../upm.i"
%include "stdint.i"
%include "cpointer.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "bacnetmstp.hpp"
%{
    #include "bacnetmstp.hpp"
%}
