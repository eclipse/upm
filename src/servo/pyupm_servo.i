// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_servo
%include "../upm.i"

%include "servo.h"
%{
    #include "servo.h"
%}

%include "es08a.h"
%{
    #include "es08a.h"
%}

%include "es9257.h"
%{
    #include "es9257.h"
%}
