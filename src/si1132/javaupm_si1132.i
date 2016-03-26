%module(directors="1") javaupm_si1132
%include "../upm.i"
%include "../upm/javaupm_interfaces.i"

%{
    #include "si1132.h"
%}

/*
%include "../upm/iModuleStatus.h"    
%include "../upm/iLightSensor.h"    

%feature("director") IModuleStatus;
%feature("director") ILightSensor;
*/

%include "si1132.h"
