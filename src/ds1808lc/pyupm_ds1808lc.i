%module pyupm_ds1808lc
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"


%include "iModuleStatus.hpp"
%include "iLightController.hpp"
%include "ds1808lc.hpp"
%{
    #include "ds1808lc.hpp"
%}
