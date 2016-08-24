// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_vk2828u7
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%include "vk2828u7.hpp"
%{
    #include "vk2828u7.hpp"
%}

