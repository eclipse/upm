%module(directors="1") javaupm_button
%include "../upm.i"
%include "../java_isr.i"
%{
    #include "button.hpp"
%}
%include "button.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_button");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
