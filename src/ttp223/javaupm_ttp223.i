%module (docstring="TTP223 Touch Sensor") javaupm_ttp223

%include "../upm.i"

%apply int {mraa::Edge}


%{
    #include "ttp223.hpp"
%}

%include "ttp223.hpp"

#%ignore installISR(mraa::Edge , void *, void *)

JAVA_ADD_INSTALLISR_EDGE(upm::TTP223)

JAVA_JNI_LOADLIBRARY(javaupm_ttp223)
