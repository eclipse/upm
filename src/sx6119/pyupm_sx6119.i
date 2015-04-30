%module pyupm_sx6119
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "sx6119.h"
%}

%include "sx6119.h"

