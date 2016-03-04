%module javaupm_ssd1351
%include "../upm.i"
%include "typemaps.i"
%include "stdint.i"

%ignore m_map;
%ignore font;

%include "gfx.h"
%{
    #include "gfx.h"
%}

%include "ssd1351.h"
%{
    #include "ssd1351.h"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ssd1351");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}