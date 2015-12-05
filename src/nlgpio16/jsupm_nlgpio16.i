%module jsupm_nlgpio16
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "nlgpio16.h"
%}

%include "nlgpio16.h"
%array_class(char, charArray);
