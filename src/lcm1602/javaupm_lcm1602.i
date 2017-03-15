%module javaupm_lcm1602
%include "../upm.i"
%include "../upm_vectortypes.i"
%include "typemaps.i"

%include "lcm1602.hpp"
%{
    #include "lcm1602.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lcm1602");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
