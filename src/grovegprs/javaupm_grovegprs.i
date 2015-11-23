%module javaupm_grovegprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "grovegprs.h"
%}

%include "grovegprs.h"
%array_class(char, charArray);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovegprs");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}