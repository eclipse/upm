%module pyupm_my9221
%include "../upm.i"

%feature("autodoc", "3");

%include "my9221.h"
%{
    #include "my9221.h"
%}
