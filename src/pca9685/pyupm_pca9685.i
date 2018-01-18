// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_pca9685
%include "../upm.i"

%include "pca9685.hpp"
%{
    #include "pca9685.hpp"
%}
