%module(directors="1") javaupm_rpr220
%include "../upm.i"


%feature("director") IsrCallback;
SWIG_DIRECTOR_OWNED(IsrCallback)
%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "rpr220.h"
%}
%include "rpr220.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rpr220");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}