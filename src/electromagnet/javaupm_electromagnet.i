%module javaupm_electromagnet
%include "../upm.i"

%{
    #include "electromagnet.hpp"
%}

%include "electromagnet.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_electromagnet");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}