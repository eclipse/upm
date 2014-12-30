%module pyupm_ublox6
%include "../upm.i"

%feature("autodoc", "3");

%include "ublox6.h"
%{
    #include "ublox6.h"
%}
