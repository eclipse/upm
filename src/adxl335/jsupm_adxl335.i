%module jsupm_adxl335
%include "../upm.i"
%include "stdint.i"
%include "carrays.i"

%array_class(int, intPointer);
%array_class(float, floatPointer);

%{
    #include "adxl335.h"
%}

%include "adxl335.h"
