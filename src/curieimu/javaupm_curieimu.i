%module javaupm_curieimu
%include "../upm.i"

%{
    #include "curieimu.hpp"
%}

%include "curieimu.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_curieimu");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
