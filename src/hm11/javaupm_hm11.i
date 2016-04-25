%module javaupm_hm11
%include "../upm.i"
%include "carrays.i"
%include "../java_buffer.i"

%{
    #include "hm11.hpp"
    speed_t int_B9600 = B9600;
%}

%include "hm11.hpp"
speed_t int_B9600 = B9600;
%array_class(char, charArray);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hm11");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}