// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bmp280
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

#ifdef DOXYGEN
%include "bmp280_doc.i"
#endif

%include "iModuleStatus.hpp"
%include "iHumiditySensor.hpp"
%include "iPressureSensor.hpp"
%include "iTemperatureSensor.hpp"

%include "bmp280_regs.h"
%include "bmp280.hpp"
%{
    #include "bmp280.hpp"
%}

%include "bme280.hpp"
%{
    #include "bme280.hpp"
%}
