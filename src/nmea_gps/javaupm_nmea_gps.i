%module javaupm_nmea_gps
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "nmea_gps.hpp"
%{
    #include "nmea_gps.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_nmea_gps)
