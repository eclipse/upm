%module jsupm_xbee
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "xbee.hpp"
%}

%include "xbee.hpp"
%array_class(char, charArray);
