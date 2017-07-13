// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mlx90614
%include "../upm.i"

%include "mlx90614.hpp"
%{
    #include "mlx90614.hpp"
%}
