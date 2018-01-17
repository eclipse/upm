%module javaupm_max44009

%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%import "../interfaces/javaupm_iLightSensor.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "max44009.hpp"
%}

%include "max44009.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_max44009)
