%module javaupm_rfr359f
%include "../upm.i"

%{
    #include "rfr359f.hpp"
%}

%include "rfr359f.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rfr359f");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}