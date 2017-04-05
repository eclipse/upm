%module javaupm_interfaces

%include javaupm_iModuleStatus.i
%include javaupm_iADC.i
%include javaupm_iCO2Sensor.i
%include javaupm_iHumiditySensor.i
%include javaupm_iLightController.i
%include javaupm_iLightSensor.i
%include javaupm_iPressureSensor.i
%include javaupm_iTemperatureSensor.i


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
