%module javaupm_mlx90614
%include "../upm.i"

%{
    #include "mlx90614.hpp"
%}

%include "mlx90614.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mlx90614)