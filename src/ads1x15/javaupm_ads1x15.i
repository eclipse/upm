%module javaupm_ads1x15
%include "../upm.i"
%include "typemaps.i"

%import "../interfaces/javaupm_iADC.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "ads1x15.hpp"
    #include "ads1015.hpp"
    #include "ads1115.hpp"
%}

%include "ads1x15.hpp"
%include "ads1015.hpp"
%include "ads1115.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ads1x15)
