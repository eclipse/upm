// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ads1x15
%include "../upm.i"

%feature("autodoc", "3");

%include "ads1x15.hpp"
%{
    #include "ads1x15.hpp"
%}

%include "ads1015.hpp"
%{
    #include "ads1015.hpp"
%}

%include "ads1115.hpp"
%{
    #include "ads1115.hpp"
%}