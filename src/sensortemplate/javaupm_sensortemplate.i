/* Specify the name of the target JAVA module */
%module javaupm_sensortemplate

/* Include the base sensor interface file */
%include "common.i"

/* Add the JAVA code to load the module */
JAVA_JNI_LOADLIBRARY(javaupm_sensortemplate)
