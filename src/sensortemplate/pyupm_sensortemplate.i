/* Specify the name of the target JAVA module */
%module pyupm_sensortemplate

/* If documentation is enabled, a full "pyupm_doxy2swig.i file will be
 * generated and used...  Otherwise, use a stub */
%include "pyupm_doxy2swig.i"

/* Include the base sensor interface file */
%include "common.i"
