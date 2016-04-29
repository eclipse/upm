%module javaupm_ds2413
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "ds2413.hpp"
%}

%include "ds2413.hpp"
%array_class(char, charArray);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ds2413");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
