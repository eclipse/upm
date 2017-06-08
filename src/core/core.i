%include "../common_top.i"

#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_core)

/* JAVA has multiple define collisions once these get flattened out,
   so ignore the base implementation */
%ignore "upm::iUpmObject::JsonDefinition";
#endif

%include ".iUpmObject.i"
%include ".iSensorType.i"
%include ".iTemperatureSensor.i"
%include ".iHumiditySensor.i"
%include ".iMraa.i"
%include ".iADC.i"
%include ".iPressureSensor.i"
%include ".iMoistureSensor.i"
%include ".iLightSensor.i"
%include ".iLightController.i"
%include ".iCO2Sensor.i"

/* Actuator types */
%include ".iActuatorType.i"
%include ".iServoActuator.i"
