%module pyupm_ads1x15
%include "../upm.i"


%feature("autodoc", "3");

%include "ads1x15.h"
%{
    #include "ads1x15.h"
%}

%include "ads1015.h"
%{
    #include "ads1015.h"
%}

%include "ads1115.h"
%{
    #include "ads1115.h"
%}