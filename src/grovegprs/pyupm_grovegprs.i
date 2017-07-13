// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovegprs
%include "../upm.i"
%include "carrays.i"
%include "std_string.i"

%{
    #include "grovegprs.hpp"
%}
%include "grovegprs.hpp"
%array_class(char, charArray);
