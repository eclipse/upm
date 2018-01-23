%module javaupm_speaker
%include "../upm.i"

%{
    #include "speaker.hpp"
%}

%include "speaker.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_speaker)