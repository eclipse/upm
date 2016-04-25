// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_xbee
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "xbee.hpp"
%}
%include "xbee.hpp"
%array_class(char, charArray);
