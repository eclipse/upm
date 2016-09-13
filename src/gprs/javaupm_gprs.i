%module javaupm_gprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "gprs.hpp"
%}

%include "gprs.hpp"
%array_class(char, charArray);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_gprs");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}