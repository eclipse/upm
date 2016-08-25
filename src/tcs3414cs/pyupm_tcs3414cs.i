// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_tcs3414cs
%include "../upm.i"

%feature("autodoc", "3");

%include "tcs3414cs.hpp"
%{
    #include "tcs3414cs.hpp"
%}
