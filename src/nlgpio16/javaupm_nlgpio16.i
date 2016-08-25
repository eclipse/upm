%module javaupm_nlgpio16
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "nlgpio16.hpp"
%}

%include "nlgpio16.hpp"
%array_class(char, charArray);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_nlgpio16");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
