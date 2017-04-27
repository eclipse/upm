%module jsupm_bmp280
%include "../upm.i"
%include "cpointer.i"

%include "iUPMObject.hpp"
%include "iHumidity.hpp"
%include "iPressure.hpp"
%include "iTemperature.hpp"

%include "bmp280_regs.h"
%include "bmp280.hpp"
%{
    #include "bmp280.hpp"
%}

%include "bme280.hpp"
%{
    #include "bme280.hpp"
%}
