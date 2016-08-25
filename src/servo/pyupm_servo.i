// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_servo
%include "../upm.i"

%include "servo.hpp"
%{
    #include "servo.hpp"
%}

%include "es08a.hpp"
%{
    #include "es08a.hpp"
%}

%include "es9257.hpp"
%{
    #include "es9257.hpp"
%}
