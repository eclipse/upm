%module pyupm_ds18b20
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "ds18b20.hpp"
%}
%include "ds18b20.hpp"
%array_class(char, charArray);
