%module jsupm_bmp280
%include "../upm.i"
%include "cpointer.i"

%include "bmp280.hpp"
%{
    #include "bmp280.hpp"
%}

%include "bme280.hpp"
%{
    #include "bme280.hpp"
%}
