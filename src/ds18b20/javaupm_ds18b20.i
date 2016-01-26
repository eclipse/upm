%module javaupm_ds18b20
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "ds18b20.h"
%}

%include "ds18b20.h"
%array_class(char, charArray);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ds18b20");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
