// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_sx6119
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "sx6119.hpp"
%}

%include "sx6119.hpp"

