%module javaupm_htu21d
%include "../upm.i"
%include "typemaps.i"

%ignore getHumidityData(float*, float*, float*);

%{
    #include "htu21d.hpp"
%}
%include "htu21d.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_htu21d)
