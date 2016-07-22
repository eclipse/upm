// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ublox6
%include "../upm.i"
%include "stdint.i"
%include "carrays.i"

%feature("autodoc", "3");

%{
    #include "ublox6.hpp"
    speed_t int_B2400 = B2400;
    speed_t int_B4800 = B4800;
    speed_t int_B9600 = B9600;
    speed_t int_B19200 = B19200;
    speed_t int_B38400 = B38400;
%}

%include "ublox6.hpp"
speed_t int_B2400 = B2400;
speed_t int_B4800 = B4800;
speed_t int_B9600 = B9600;
speed_t int_B19200 = B19200;
speed_t int_B38400 = B38400;
%array_class(char, charArray);
