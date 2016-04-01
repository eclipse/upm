%module javaupm_e50hx
%include "../upm.i"
%include "typemaps.i"

%include "bacnetmstp.h"
%include "e50hx.h"
%{
    #include "e50hx.h"
%}


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_e50hx");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
