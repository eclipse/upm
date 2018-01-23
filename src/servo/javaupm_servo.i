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

JAVA_JNI_LOADLIBRARY(javaupm_servo)