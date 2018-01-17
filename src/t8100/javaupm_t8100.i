%module javaupm_t8100
%include "../upm.i"
%include "typemaps.i"

%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"
%include "t8100.hpp"
%{
    #include "t8100.hpp"
%}


JAVA_JNI_LOADLIBRARY(javaupm_t8100)
