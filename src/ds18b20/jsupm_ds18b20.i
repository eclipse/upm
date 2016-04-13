%module jsupm_ds18b20
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "ds18b20.h"
%}

%include "ds18b20.h"
%array_class(char, charArray);
