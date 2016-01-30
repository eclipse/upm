// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_st7735
%include "../upm.i"

%feature("autodoc", "3");
%rename("printString") print(std::string msg);

%include "gfx.h"
%{
    #include "gfx.h"
%}

%include "st7735.h"
%{
    #include "st7735.h"
%}
