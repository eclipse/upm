%module javaupm_button
%include "../upm.i"

%apply int {mraa::Edge}

%{
    #include "button.hpp"
%}

%include "button.hpp"

#%ignore

JAVA_ADD_INSTALLISR_EDGE(upm::Button)

JAVA_JNI_LOADLIBRARY(javaupm_button)
