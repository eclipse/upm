%module pyupm_ds2413
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "ds2413.h"
%}
%include "ds2413.h"
%array_class(char, charArray);
