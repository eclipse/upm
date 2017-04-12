%module javaupm_lcdks
%include "../upm.i"
%include "../upm_vectortypes.i"
%include "typemaps.i"

%include "lcdks.hpp"
%{
    #include "lcdks.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lcdks");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
