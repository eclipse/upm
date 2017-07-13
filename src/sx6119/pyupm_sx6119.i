// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_sx6119
%include "../upm.i"

%{
    #include "sx6119.hpp"
%}

%include "sx6119.hpp"

