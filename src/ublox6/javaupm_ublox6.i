%module javaupm_ublox6
%include "../upm.i"
%include "../java_buffer.i"

%{
    #include "ublox6.h"
    speed_t int_B9600 = B9600;
%}

%include "ublox6.h"
speed_t int_B9600 = B9600;
