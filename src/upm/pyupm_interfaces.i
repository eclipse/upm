%module pyupm_interfaces
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "../upm/iLightSensor.h"
    #include "../upm/iLightController.h"
%}

%include "iModuleStatus.h"
%include "iLightSensor.h"
%include "iLightController.h"
