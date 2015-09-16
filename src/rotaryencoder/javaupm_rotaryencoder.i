%module javaupm_rotaryencoder
%include "../upm.i"

%ignore signalAISR;

%{
    #include "rotaryencoder.h"
%}

%include "rotaryencoder.h"
