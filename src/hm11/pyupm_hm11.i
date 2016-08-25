// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hm11
%include "../upm.i"
%include "carrays.i"

%feature("autodoc", "3");

%{
    #include "hm11.hpp"
    speed_t int_B9600 = B9600;
%}
%include "hm11.hpp"
speed_t int_B9600 = B9600;
%array_class(char, charArray);
