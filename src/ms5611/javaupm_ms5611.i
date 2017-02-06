%module javaupm_ms5611
%include "../upm.i"

#if SWIG_VERSION >= 0x030009
%include "../interfaces/javaupm_iTemperatureSensor.i"
%include "../interfaces/javaupm_iPressureSensor.i"
#endif

%{
    #include "ms5611.hpp"
%}

%include "ms5611.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ms5611");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
