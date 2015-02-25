%module pyupm_pca9685
%include "../upm.i"

%feature("autodoc", "3");

%include "pca9685.h"
%{
    #include "pca9685.h"
%}
