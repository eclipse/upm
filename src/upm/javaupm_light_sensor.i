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

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_light_sensor");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
