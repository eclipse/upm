%module javaupm_ssd1351
%include "../upm.i"
%include "typemaps.i"
%include "stdint.i"

%ignore m_map;
%ignore font;

%include "ssd1351_gfx.hpp"
%{
    #include "ssd1351_gfx.hpp"
%}

%include "ssd1351.hpp"
%{
    #include "ssd1351.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_ssd1351)