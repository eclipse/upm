%module (directors=1, docstring="TTP223 Touch Sensor") javaupm_ttp223

%include "../upm.i"

%feature("director") IsrCallback;
SWIG_DIRECTOR_OWNED(IsrCallback)
%ignore generic_callback_isr;
%include "../IsrCallback.h"

%apply int {mraa::Edge}


%{
    #include "ttp223.h"
%}

%include "ttp223.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ttp223");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}