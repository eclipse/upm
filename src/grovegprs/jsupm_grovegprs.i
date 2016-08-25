%module jsupm_grovegprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "grovegprs.hpp"
%}

%include "grovegprs.hpp"
%array_class(char, charArray);
