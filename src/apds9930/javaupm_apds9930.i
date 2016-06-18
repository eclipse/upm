%module javaupm_apds9930
%include "../upm.i"

%{
    #include "apds9930.hpp"
%}
%include "apds9930.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_apds9930");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
