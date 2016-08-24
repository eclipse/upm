%module javaupm_bh1750
%include "../upm.i"
%include "std_string.i"

%{
    #include "bh1750.hpp"
%}

%include "bh1750.hpp"


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bh1750");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
