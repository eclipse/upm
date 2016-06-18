%module javaupm_servo
%include "../upm.i"

%include "servo.hpp"
%{
    #include "servo.hpp"
%}

%include "es08a.hpp"
%{
    #include "es08a.hpp"
%}

%include "es9257.hpp"
%{
    #include "es9257.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_servo");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}