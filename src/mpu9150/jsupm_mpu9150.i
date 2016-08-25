%module jsupm_mpu9150
%include "../upm.i"
%include "cpointer.i"

%pointer_functions(float, floatp);

%{
    #include "mpu9150.hpp"
%}

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

