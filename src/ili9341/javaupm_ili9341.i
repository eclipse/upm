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

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ili9341");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}