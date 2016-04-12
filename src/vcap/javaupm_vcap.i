%module javaupm_vcap
%include "../upm.i"
%include "std_string.i"

%include "vcap.hpp"
%{
    #include "vcap.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_vcap");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
