%module(directors="1") javaupm_interfaces

%feature("director") IModuleStatus;
%feature("director") ILightSensor;
%feature("director") ILightController;


%{
    #include "../upm/iLightSensor.h"
    #include "../upm/iLightController.h"    
%}

%include "iModuleStatus.h"    
%include "iLightSensor.h"    
%include "iLightController.h"    
