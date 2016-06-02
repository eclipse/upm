// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_t8100
%include "../upm.i"
%include "stdint.i"

%feature("autodoc", "3");

%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"
%include "t8100.hpp"
%{
    #include "t8100.hpp"
%}
