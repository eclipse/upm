// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mpu9150
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "ak8975.hpp"
%{
    #include "ak8975.hpp"
%}

%include "mpu60x0.hpp"
%{
    #include "mpu60x0.hpp"
%}

%include "mpu9150.hpp"
%{
    #include "mpu9150.hpp"
%}

%include "mpu9250.hpp"
%{
    #include "mpu9250.hpp"
%}

