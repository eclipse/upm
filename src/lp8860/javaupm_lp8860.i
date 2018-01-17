%module javaupm_lp8860

%import "../interfaces/javaupm_iLightController.i"

%include "../upm.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "lp8860.hpp"
%}

%include "lp8860.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_lp8860)
