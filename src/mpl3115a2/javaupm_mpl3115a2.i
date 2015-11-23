%module javaupm_mpl3115a2
%include "../upm.i"

%{
    #include "mpl3115a2.h"
%}

%include "mpl3115a2.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mpl3115a2");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}