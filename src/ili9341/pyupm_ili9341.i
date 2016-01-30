// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ili9341
%include "../upm.i"

%feature("autodoc", "3");
%rename("printString") print(std::string msg);

%include "gfx.h"
%{
    #include "gfx.h"
%}

%include "ili9341.h"
%{
    #include "ili9341.h"
%}
