%module jsupm_interfaces
%include "../upm.i"

%{
    #include "../upm/iLightSensor.hpp"
    #include "../upm/iLightController.hpp"
%}

%include "iModuleStatus.hpp"
%include "iLightSensor.hpp"
%include "iLightController.hpp"
