%module javaupm_kxcjk1013
%include "../upm.i"

%{
    #include "kxcjk1013.h"
%}
%include "kxcjk1013.h"

%ignore installISR(void (*isr)(char*), void* arg);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_kxcjk1013");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
