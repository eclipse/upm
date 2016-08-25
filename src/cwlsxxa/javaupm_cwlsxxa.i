%module javaupm_cwlsxxa
%include "../upm.i"
%include "std_string.i"

%include "cwlsxxa.hpp"
%{
    #include "cwlsxxa.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_cwlsxxa");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
