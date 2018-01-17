%module javaupm_ds1808lc
%include "../upm.i"

%import "../interfaces/javaupm_iLightController.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%{
    #include "ds1808lc.hpp"
%}

%include "ds1808lc.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ds1808lc)
