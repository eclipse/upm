%module javaupm_ppd42ns
%include "../upm.i"

%include "ppd42ns_data.h"
%include "ppd42ns.hpp"
%{
    #include "ppd42ns.hpp"
%}


JAVA_JNI_LOADLIBRARY(javaupm_ppd42ns)
