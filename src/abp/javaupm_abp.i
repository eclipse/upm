%module javaupm_abp
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%{
    #include "abp.hpp"
%}
%include "abp.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_abp");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
