%module pyupm_ppd42ns
%include "../upm.i"

%feature("autodoc", "3");

%include "ppd42ns.h"
%{
    #include "ppd42ns.h"
%}
