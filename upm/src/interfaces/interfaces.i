%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%import "../_upm.i"

%include javaupm_iModuleStatus.i
%include javaupm_iADC.i
%include javaupm_iCO2Sensor.i
%include javaupm_iHumiditySensor.i
%include javaupm_iLightController.i
%include javaupm_iLightSensor.i
%include javaupm_iPressureSensor.i
%include javaupm_iTemperatureSensor.i

JAVA_JNI_LOADLIBRARY(javaupm_interfaces)
#endif
/* END Java syntax */
