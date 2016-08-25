%module javaupm_xbee
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "xbee.hpp"
%}

%include "xbee.hpp"
%array_class(char, charArray);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_xbee");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}