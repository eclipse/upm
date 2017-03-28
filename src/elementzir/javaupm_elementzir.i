%module javaupm_elementzir
%include "../upm.i"

%{
    #include "elementzir.hpp"
%}

%include "elementzir.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_elementzir");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}

