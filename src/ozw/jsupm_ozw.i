%module jsupm_ozw
%include "../upm.i"
%include "cpointer.i"
%include "stdint.i"

%pointer_functions(float, floatp);

%include "ozw.hpp"
%{
    #include "ozw.hpp"
%}

%include "ozwinterface.hpp"
%{
    #include "ozwinterface.hpp"
%}

%include "ozwdump.hpp"
%{
    #include "ozwdump.hpp"
%}

%include "aeotecss6.hpp"
%{
    #include "aeotecss6.hpp"
%}

%include "aeotecsdg2.hpp"
%{
    #include "aeotecsdg2.hpp"
%}

%include "aeotecdw2e.hpp"
%{
    #include "aeotecdw2e.hpp"
%}

%include "aeotecdsb09104.hpp"
%{
    #include "aeotecdsb09104.hpp"
%}

%include "tzemt400.hpp"
%{
    #include "tzemt400.hpp"
%}
