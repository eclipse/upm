%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../java_buffer.i"
%include "../java_exceptions.i"

%apply uint32_t *OUTPUT { uint32_t *freq, uint32_t *dataRate };
%apply uint16_t *OUTPUT { uint16_t *rxBandwidth };
%apply uint8_t *OUTPUT { uint8_t *modulation, uint8_t *txPower };
%apply uint32_t *OUTPUT { uint32_t *uartBaud };
%apply uint8_t *OUTPUT { uint8_t *strength };

%ignore getRFSignalStrength(uint8_t *strength);
READDATA_EXCEPTION(getRFSignalStrength())

%ignore getModSignalStrength(uint8_t *strength);
READDATA_EXCEPTION(getModSignalStrength())

JAVA_JNI_LOADLIBRARY(javaupm_hmtrp)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"

%array_class(char, charArray);
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"

%rename("getModSignalStrengthNoParam")  getModSignalStrength();
%rename("getRFSignalStrengthNoParam") getRFSignalStrength();

%array_class(char, charArray);
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "hmtrp.hpp"
speed_t int_B9600 = B9600;
%}
%include "hmtrp.hpp"

speed_t int_B9600 = B9600;
/* END Common SWIG syntax */
