%module pyupm_maxsonarez
%include "../upm.i"

%feature("autodoc", "3");

%include "maxsonarez.h"
%{
    #include "maxsonarez.h"
%}
