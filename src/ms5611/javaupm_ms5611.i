%module javaupm_ms5611
%include "../upm.i"

%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iPressureSensor.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "ms5611.hpp"
%}

%include "ms5611.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ms5611)
