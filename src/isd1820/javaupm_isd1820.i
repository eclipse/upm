%module javaupm_isd1820
%include "../upm.i"

%{
    #include "isd1820.hpp"
%}

%include "isd1820.hpp"


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_isd1820");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}