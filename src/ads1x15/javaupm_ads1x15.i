%module javaupm_ads1x15
%include "../upm.i"
%include "typemaps.i"

%{
    #include "ads1x15.h"
    #include "ads1015.h"
    #include "ads1115.h"
%}

%include "ads1x15.h"
%include "ads1015.h"
%include "ads1115.h"



%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ads1x15");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}