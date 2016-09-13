%module jsupm_grove
%include "../upm.i"

%include "grove.hpp"
%{
    #include "grove.hpp"
%}

%include "grovebase.hpp"
%{
    #include "grovebase.hpp"
%}

%include "button.hpp"
%{
    #include "button.hpp"
%}

%include "groveled.hpp"
%{
    #include "groveled.hpp"
%}

%include "grovetemp.hpp"
%{
    #include "grovetemp.hpp"
%}
