%module pyupm_interfaces
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "../upm/iLightSensor.hpp"
    #include "../upm/iLightController.hpp"
%}

%include "iModuleStatus.hpp"
%include "iLightSensor.hpp"
%include "iLightController.hpp"
