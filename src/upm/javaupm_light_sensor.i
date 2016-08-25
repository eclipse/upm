%module(directors="1") javaupm_light_sensor

%{
    #include "iLightSensor.hpp"
%}

/*
%include "../upm/iModuleStatus.hpp"    
*/
%include "iLightSensor.hpp"    

%feature("director") IModuleStatus;
%feature("director") ILightSensor;

%include "iLightSensor.hpp"
