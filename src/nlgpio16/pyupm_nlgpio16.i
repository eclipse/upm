// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_nlgpio16
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "nlgpio16.hpp"
%}
%include "nlgpio16.hpp"
%array_class(char, charArray);
