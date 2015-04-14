%module pyupm_grovewfs
%include "../upm.i"

%feature("autodoc", "3");

%include "grovewfs.h"
%{
    #include "grovewfs.h"
%}
