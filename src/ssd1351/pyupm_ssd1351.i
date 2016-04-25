// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ssd1351
%include "../upm.i"

%feature("autodoc", "3");
%rename("printString") print(std::string msg);

%include "ssd1351_gfx.hpp"
%{
    #include "ssd1351_gfx.hpp"
%}

%include "ssd1351.hpp"
%{
    #include "ssd1351.hpp"
%}
