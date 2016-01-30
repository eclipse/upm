%module javaupm_ili9341
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%apply uint8_t *INPUT { uint8_t *addr }

%include "gfx.h"
%{
    #include "gfx.h"
%}

%{
    #include "ili9341.h"
%}

%include "ili9341.h"

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