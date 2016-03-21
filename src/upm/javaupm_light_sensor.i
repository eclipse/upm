%module(directors="1") javaupm_light_sensor

%{
    #include "iLightSensor.h"
%}

/*
%include "../upm/iModuleStatus.h"    
*/
%include "iLightSensor.h"    

%feature("director") IModuleStatus;
%feature("director") ILightSensor;

%include "iLightSensor.h"
