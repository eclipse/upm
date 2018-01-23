%module javaupm_buzzer
%include "../upm.i"


%include "buzzer_tones.h"
%include "buzzer.hpp"
%{
    #include "buzzer.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_buzzer)
