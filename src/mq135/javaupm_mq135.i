%module javaupm_mq135
%include "../upm.i"

%{
    #include "mq135.h"
%}

%include "mq135.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq135");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
