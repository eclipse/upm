%module pyupm_buzzer
%include "../upm.i"

%feature("autodoc", "3");

%include "buzzer.h"
%{
    #include "buzzer.h"
%}
