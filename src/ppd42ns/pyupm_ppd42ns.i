// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ppd42ns
%include "../upm.i"

%include "ppd42ns_data.h"
%include "ppd42ns.hpp"
%{
    #include "ppd42ns.hpp"
%}
