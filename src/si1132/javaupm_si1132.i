%module(directors="1") javaupm_si1132
%include "../upm.i"
%include "../upm/javaupm_interfaces.i"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_si1132");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}

%{
    #include "si1132.hpp"
%}

/*
%include "../upm/iModuleStatus.hpp"
%include "../upm/iLightSensor.hpp"

%feature("director") IModuleStatus;
%feature("director") ILightSensor;
*/

%include "si1132.hpp"

