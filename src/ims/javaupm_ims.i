%module javaupm_ims
%include "../upm.i"
%include "enums.swg"

%javaconst(1);
enum IMS_RD_COMMAND {
	/* Read capacitance (moisture) register */
    IMS_GET_CAPACITANCE = 0x00, /*    (r) 2 */
    /* Read I2C address register */
    IMS_GET_ADDRESS     = 0x02, /*    (r) 1 */
    /* Read light register (requires write to IMS_MEASURE_LIGHT) */
    IMS_GET_LIGHT       = 0x04, /*    (r) 2 */
    /* Read temperature register */
    IMS_GET_TEMPERATURE = 0x05, /*    (r) 2 */
    /* Read version register */
    IMS_GET_VERSION     = 0x07, /*    (r) 1 */
    /* Read busy register (0 = ready, 1 = sampling) */
    IMS_GET_BUSY        = 0x09, /*    (r) 1 */
};

enum IMS_WR_COMMAND {
	/* Write I2C address register (latched w/IMS_RESET) */
    IMS_SET_ADDRESS     = 0x01, /*    (w) 1 */
    /* Initiate light measurement */
    IMS_MEASURE_LIGHT   = 0x03, /*    (w) 0 */
    /* Reset device */
    IMS_RESET           = 0x06, /*    (w) 0 */
    /* Sleep microcontroller, wake on any I2C request */
    IMS_SLEEP           = 0x08, /*    (w) 0 */
};

%{
    #include "ims.hpp"
%}

%include "ims.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ims");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
