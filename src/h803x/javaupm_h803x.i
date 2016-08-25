%module javaupm_h803x
%include "../upm.i"
%include "typemaps.i"

%{
    #include "h803x.hpp"
%}

%include "h803x.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_h803x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
