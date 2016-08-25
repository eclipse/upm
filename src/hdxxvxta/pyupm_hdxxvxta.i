// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hdxxvxta
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "hdxxvxta.hpp"
%}
%include "hdxxvxta.hpp"

