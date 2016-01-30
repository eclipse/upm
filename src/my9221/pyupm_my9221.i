// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_my9221
%include "../upm.i"

%feature("autodoc", "3");

%include "my9221.h"
%{
    #include "my9221.h"
%}

%include "groveledbar.h"
%{
    #include "groveledbar.h"
%}

%include "grovecircularled.h"
%{
    #include "grovecircularled.h"
%}
