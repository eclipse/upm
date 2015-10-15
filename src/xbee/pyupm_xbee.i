%module pyupm_xbee
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "xbee.h"
%}
%include "xbee.h"
%array_class(char, charArray);
