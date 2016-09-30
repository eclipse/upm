%module(directors="1") javaupm_interfaces

%feature("director") IModuleStatus;
%feature("director") ILightSensor;
%feature("director") ILightController;


%{
    #include "../upm/iLightSensor.hpp"
    #include "../upm/iLightController.hpp"
%}

%include "iModuleStatus.hpp"
%include "iLightSensor.hpp"
%include "iLightController.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_interfaces");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
