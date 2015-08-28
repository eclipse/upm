%module pyupm_i2clcd
%include "../upm.i"
%include "../carrays_uint8_t.i"

%typemap(in) uint8_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint8Array, 0 |  0 );
  $1 = reinterpret_cast< uint8_t * >(argp);
}


%feature("autodoc", "3");

%include "ssd.h"
%include "lcd.h"
%{
    #include "lcd.h"
%}

%include "lcm1602.h"
%{
    #include "lcm1602.h"
%}

%include "jhd1313m1.h"
%{
    #include "jhd1313m1.h"
%}

%include "ssd1327.h"
%{
    #include "ssd1327.h"
%}

%include "ssd1308.h"
%{
    #include "ssd1308.h"
%}

%include "eboled.h"
%{
    #include "eboled.h"
%}

%include "sainsmartks.h"
%{
    #include "sainsmartks.h"
%}

%include "ssd1306.h"
%{
    #include "ssd1306.h"
%}
