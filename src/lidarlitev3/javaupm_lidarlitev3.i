%module javaupm_lidarlitev3
%include "../upm.i"

%{
    #include "lidarlitev3.hpp"
%}

%include "lidarlitev3.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lidarlitev3");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
