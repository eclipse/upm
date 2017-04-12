%module javaupm_ecs1030
%include "../upm.i"

%{
    #include "ecs1030.hpp"
%}

%include "ecs1030.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ecs1030");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}