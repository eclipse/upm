%module javaupm_hmtrp
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "../java_buffer.i"
%include "../java_exceptions.i"

%apply uint32_t *OUTPUT { uint32_t *freq, uint32_t *dataRate };
%apply uint16_t *OUTPUT { uint16_t *rxBandwidth };
%apply uint8_t *OUTPUT { uint8_t *modulation, uint8_t *txPower };
%apply uint32_t *OUTPUT { uint32_t *uartBaud };
%apply uint8_t *OUTPUT { uint8_t *strength };

%{
    #include "hmtrp.hpp"
    speed_t int_B9600 = B9600;
%}

%ignore getRFSignalStrength(uint8_t *strength);
READDATA_EXCEPTION(getRFSignalStrength())

%ignore getModSignalStrength(uint8_t *strength);
READDATA_EXCEPTION(getModSignalStrength())

%include "hmtrp.hpp"
speed_t int_B9600 = B9600;

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hmtrp");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
