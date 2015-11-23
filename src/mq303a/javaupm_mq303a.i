%module javaupm_mq303a
%include "../upm.i"

%{
    #include "mq303a.h"
%}

%include "mq303a.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mq303a");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}