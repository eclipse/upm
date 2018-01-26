%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../java_buffer.i"
%include "../java_exceptions.i"

%apply uint8_t *OUTPUT { uint8_t *vol };
%apply uint8_t *OUTPUT { uint8_t *ps };
%apply uint16_t *OUTPUT { uint16_t *numf };
%apply uint16_t *OUTPUT { uint16_t *curf };
%apply uint16_t *OUTPUT { uint16_t *year };
%apply uint8_t *OUTPUT { uint8_t *month, uint8_t *day };
%apply uint8_t *OUTPUT { uint8_t *hour, uint8_t *minute, uint8_t *second };

%ignore getVolume(uint8_t *vol);
READDATA_EXCEPTION(getVolume())

%ignore getPlayState(uint8_t *ps);
READDATA_EXCEPTION(getPlayState())

%ignore getNumFiles(WT5001_PLAYSOURCE_T psrc, uint16_t *numf);
READDATA_EXCEPTION(getNumFiles(WT5001_PLAYSOURCE_T psrc))

%ignore getCurrentFile(uint16_t *curf);
READDATA_EXCEPTION(getCurrentFile())

JAVA_JNI_LOADLIBRARY(javaupm_wt5001)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%rename("getVolumeNoParam")  getVolume();
%rename("getPlayStateNoParam") getPlayState();
%rename("getNumFilesOneParam")  getNumFiles(WT5001_PLAYSOURCE_T psrc);
%rename("getCurrentFileNoParam") getCurrentFile();
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"

%{
#include "wt5001.hpp"
speed_t int_B9600 = B9600;
%}
%include "wt5001.hpp"
speed_t int_B9600 = B9600;
/* END Common SWIG syntax */
