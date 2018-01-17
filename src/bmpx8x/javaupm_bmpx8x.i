%module javaupm_bmpx8x
%include "../upm.i"

%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iPressureSensor.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "bmpx8x.hpp"
%}

%include "bmpx8x_defs.h"
%include "bmpx8x.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_bmpx8x)
