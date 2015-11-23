%module javaupm_servo
%include "../upm.i"

%include "servo.h"
%{
    #include "servo.h"
%}

%include "es08a.h"
%{
    #include "es08a.h"
%}

%include "es9257.h"
%{
    #include "es9257.h"
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