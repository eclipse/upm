%module javaupm_mlx90614
%include "../upm.i"

%{
    #include "mlx90614.h"
%}

%include "mlx90614.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mlx90614");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}