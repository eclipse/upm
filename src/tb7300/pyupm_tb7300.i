// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_tb7300
%include "../upm.i"
%include "stdint.i"

%feature("autodoc", "3");

%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"
%include "tb7300.hpp"
%{
    #include "tb7300.hpp"
%}
