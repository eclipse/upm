%module jsupm_gprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "gprs.hpp"
%}

%include "gprs.hpp"
%array_class(char, charArray);
