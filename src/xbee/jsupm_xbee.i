%module jsupm_xbee
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "xbee.h"
%}

%include "xbee.h"
%array_class(char, charArray);
