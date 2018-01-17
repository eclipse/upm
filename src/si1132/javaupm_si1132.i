#ifdef ANDROID
    %module javaupm_si1132
#else
    %module(directors="1") javaupm_si1132
#endif

%include "../upm.i"

%import "../interfaces/javaupm_iLightSensor.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_si1132)

%include "si1132.hpp"
%{
    #include "si1132.hpp"
%}


