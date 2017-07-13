// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_relay
%include "../upm.i"

%include "relay.hpp"
%{
    #include "relay.hpp"
%}
