%module pyupm_urm37
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "urm37.h"
%}
%include "urm37.h"

