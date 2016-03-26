%module jsupm_interfaces
%include "../upm.i"

%{
    #include "../upm/iLightSensor.h"
    #include "../upm/iLightController.h"
%}

%include "iModuleStatus.h"
%include "iLightSensor.h"
%include "iLightController.h"
