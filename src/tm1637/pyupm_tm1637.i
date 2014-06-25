%module pyupm_tm1637
%include "../upm.i"

%feature("autodoc", "3");

%include "tm1637.h"
%{
    #include "tm1637.h"
%}
