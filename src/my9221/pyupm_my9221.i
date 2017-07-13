// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_my9221
%include "../upm.i"

%include "my9221.hpp"
%{
    #include "my9221.hpp"
%}

%include "groveledbar.hpp"
%{
    #include "groveledbar.hpp"
%}

%include "grovecircularled.hpp"
%{
    #include "grovecircularled.hpp"
%}
