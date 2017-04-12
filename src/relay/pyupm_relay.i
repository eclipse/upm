// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_relay
%include "../upm.i"

%feature("autodoc", "3");

%include "relay.hpp"
%{
    #include "relay.hpp"
%}
