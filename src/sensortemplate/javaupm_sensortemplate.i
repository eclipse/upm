/* Specify the name of the target JAVA module */
%module javaupm_sensortemplate

/* Include the base sensor interface file */
%include "common.i"

/* Add the JAVA code to load the module */
%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_sensortemplate");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
