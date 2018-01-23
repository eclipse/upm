%module javaupm_grovespeaker
%include "../upm.i"

%{
    #include "grovespeaker.hpp"
%}

%include "grovespeaker.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovespeaker)