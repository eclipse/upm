%module javaupm_hlg150h

%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%import "../interfaces/javaupm_iLightController.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "hlg150h.hpp"
%}

%include "hlg150h.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_hlg150h)
