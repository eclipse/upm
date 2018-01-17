%module javaupm_ili9341
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%apply uint8_t *INPUT { uint8_t *addr }

%include "ili9341_gfx.hpp"
%{
    #include "ili9341_gfx.hpp"
%}

%{
    #include "ili9341.hpp"
%}

%include "ili9341.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ili9341)