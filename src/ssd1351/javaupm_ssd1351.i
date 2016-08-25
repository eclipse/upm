%module javaupm_ssd1351
%include "../upm.i"
%include "typemaps.i"
%include "stdint.i"

%ignore m_map;
%ignore font;

%include "ssd1351_gfx.hpp"
%{
    #include "ssd1351_gfx.hpp"
%}

%include "ssd1351.hpp"
%{
    #include "ssd1351.hpp"
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