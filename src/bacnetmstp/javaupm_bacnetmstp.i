%module javaupm_bacnetmstp
%include "../upm.i"
%include "typemaps.i"
%include "cpointer.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "bacnetmstp.hpp"
%}

%include "bacnetmstp.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bacnetmstp");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
