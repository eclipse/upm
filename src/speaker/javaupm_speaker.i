%module javaupm_speaker
%include "../upm.i"

%{
    #include "speaker.hpp"
%}

%include "speaker.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_speaker");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}