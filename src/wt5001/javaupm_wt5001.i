%module javaupm_wt5001
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "../java_buffer.i"
%include "../java_exceptions.i"

%apply uint8_t *OUTPUT { uint8_t *vol };
%apply uint8_t *OUTPUT { uint8_t *ps };
%apply uint16_t *OUTPUT { uint16_t *numf };
%apply uint16_t *OUTPUT { uint16_t *curf };
%apply uint16_t *OUTPUT { uint16_t *year };
%apply uint8_t *OUTPUT { uint8_t *month, uint8_t *day };
%apply uint8_t *OUTPUT { uint8_t *hour, uint8_t *minute, uint8_t *second };

%{
    #include "wt5001.hpp"
    speed_t int_B9600 = B9600;
%}

%ignore getVolume(uint8_t *vol);
READDATA_EXCEPTION(getVolume())

%ignore getPlayState(uint8_t *ps);
READDATA_EXCEPTION(getPlayState())

%ignore getNumFiles(WT5001_PLAYSOURCE_T psrc, uint16_t *numf);
READDATA_EXCEPTION(getNumFiles(WT5001_PLAYSOURCE_T psrc))

%ignore getCurrentFile(uint16_t *curf);
READDATA_EXCEPTION(getCurrentFile())

%include "wt5001.hpp"
speed_t int_B9600 = B9600;

JAVA_JNI_LOADLIBRARY(javaupm_wt5001)