// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ssd1351
%include "../upm.i"

%feature("autodoc", "3");
%rename("printString") print(std::string msg);

%include "gfx.h"
%{
    #include "gfx.h"
%}

%include "ssd1351.h"
%{
    #include "ssd1351.h"
%}
