// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ublox6
%include "../upm.i"
%include "stdint.i"
%include "carrays.i"

%feature("autodoc", "3");

%{
    #include "ublox6.hpp"
    speed_t int_B9600 = B9600;
%}

%include "ublox6.hpp"
speed_t int_B9600 = B9600;
%array_class(char, charArray);
