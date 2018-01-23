%module javaupm_bacnetmstp
%include "../upm.i"
%include "typemaps.i"
%include "../carrays_uint32_t.i"

%{
    #include "bacnetmstp.hpp"
    #include "bacnetutil.hpp"
%}

%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_bacnetmstp)
