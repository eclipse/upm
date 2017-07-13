// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_i2clcd
%include "../upm.i"
%include "../carrays_uint8_t.i"

%include "ssd.hpp"
%include "lcd.hpp"
%{
    #include "lcd.hpp"
%}

%include "ssd1327.hpp"
%{
    #include "ssd1327.hpp"
%}

%include "ssd1308.hpp"
%{
    #include "ssd1308.hpp"
%}

%include "eboled.hpp"
%{
    #include "eboled.hpp"
%}

%include "ssd1306.hpp"
%{
    #include "ssd1306.hpp"
%}
