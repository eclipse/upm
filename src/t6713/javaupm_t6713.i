%module javaupm_t6713
%include "../upm.i"

%import "../interfaces/javaupm_iCO2Sensor.i"

%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "t6713.hpp"
%}

%include "t6713.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_t6713)
