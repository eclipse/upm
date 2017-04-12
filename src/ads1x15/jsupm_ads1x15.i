%module jsupm_ads1x15
%include "../upm.i"

%include "iModuleStatus.hpp"
%include "iADC.hpp"

%include "ads1x15.hpp"
%{
    #include "ads1x15.hpp"
%}

%include "ads1015.hpp"
%{
    #include "ads1015.hpp"
%}

%include "ads1115.hpp"
%{
    #include "ads1115.hpp"
%}
