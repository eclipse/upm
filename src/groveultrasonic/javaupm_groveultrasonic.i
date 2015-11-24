%module javaupm_groveultrasonic
%include "../upm.i"

%ignore signalISR;

%{
    #include "groveultrasonic.h"
%}

%include "groveultrasonic.h"
