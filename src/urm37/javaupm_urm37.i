%module javaupm_urm37
%include "../upm.i"
%include "std_string.i"

%{
    #include "urm37.h"
%}

%include "urm37.h"


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_urm37");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}