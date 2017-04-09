%module javaupm_htu21d
%include "../upm.i"
%include "typemaps.i"

%apply float *OUTPUT {float *fHum};
%apply float *OUTPUT {float *fHumTemp};
%apply float *OUTPUT {float *fDewPt};
%inline %{
    extern int getHumidityData(float* fHum, float* fHumTemp, float* fDewPt);
%}

%{
    #include "htu21d.hpp"
%}

%include "htu21d.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_htu21d");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
