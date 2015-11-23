%module(directors="1") javaupm_a110x
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "a110x.h"
%}
%include "a110x.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_a110x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}