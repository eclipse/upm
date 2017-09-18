%module javaupm_zfm20
%include "stdint.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i";

%include "common.i"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_zfm20");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
