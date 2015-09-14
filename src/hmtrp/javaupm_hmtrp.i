%module javaupm_hmtrp
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "../java_buffer.i"

%apply uint32_t *OUTPUT { uint32_t *freq, uint32_t *dataRate };
%apply uint16_t *OUTPUT { uint16_t *rxBandwidth };
%apply uint8_t *OUTPUT { uint8_t *modulation, uint8_t *txPower };
%apply uint32_t *OUTPUT { uint32_t *uartBaud };
%apply uint8_t *OUTPUT { uint8_t *strength };

%{
    #include "hmtrp.h"
    speed_t int_B9600 = B9600;
%}

%include "hmtrp.h"
speed_t int_B9600 = B9600;
