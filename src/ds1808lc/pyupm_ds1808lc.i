%module pyupm_tubelight
%include "../upm.i"

%feature("autodoc", "3");

%include "tubelight.h"
%{
    #include "tubelight.h"
%}
