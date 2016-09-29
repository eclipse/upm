%module pyupm_ds1808lc
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"


%feature("autodoc", "3");

%include "ds1808lc.hpp"
%{
    #include "ds1808lc.hpp"
%}
