%module javaupm_p9813

%include "../upm.i"
%include "typemaps.i"

%{
    #include "p9813.hpp"
%}
%include "p9813.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_p9813");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
