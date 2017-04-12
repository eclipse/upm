%module javaupm_ehr
%include "../upm.i"

%ignore beatISR;

%{
    #include "ehr.hpp"
%}

%include "ehr.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ehr");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}