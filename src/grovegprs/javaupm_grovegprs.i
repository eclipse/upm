%module javaupm_grovegprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "grovegprs.h"
%}

%include "grovegprs.h"
%array_class(char, charArray);
