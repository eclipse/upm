%module (docstring="TTP223 Touch Sensor") javaupm_ttp223

%include "../upm.i"

%apply int {mraa::Edge}


%{
    #include "ttp223.hpp"
%}

%include "ttp223.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ttp223)
