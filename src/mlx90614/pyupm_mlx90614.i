// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mlx90614
%include "../upm.i"

%feature("autodoc", "3");

%include "mlx90614.hpp"
%{
    #include "mlx90614.hpp"
%}
