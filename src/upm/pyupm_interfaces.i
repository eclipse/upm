%module pyupm_interfaces
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%feature("autodoc", "3");

%{
    #include "../upm/iLightSensor.hpp"
    #include "../upm/iLightController.hpp"
%}

%include "iModuleStatus.hpp"
%include "iLightSensor.hpp"
%include "iLightController.hpp"
