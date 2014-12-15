%module pyupm_grovespeaker
%include "../upm.i"

%feature("autodoc", "3");

%include "grovespeaker.h"
%{
    #include "grovespeaker.h"
%}
