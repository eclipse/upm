%module javaupm_e50hx
%include "../upm.i"
%include "typemaps.i"

%include "bacnetmstp.hpp"
%include "e50hx.hpp"
%{
    #include "e50hx.hpp"
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
