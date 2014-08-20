%module pyupm_mlx90614
%include "../upm.i"

%feature("autodoc", "3");

%include "mlx90614.h"
%{
    #include "mlx90614.h"
%}
