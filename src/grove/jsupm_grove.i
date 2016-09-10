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

%include "grovebutton.hpp"
%{
    #include "grovebutton.hpp"
%}

%include "groveled.hpp"
%{
    #include "groveled.hpp"
%}

%include "groverelay.hpp"
%{
    #include "groverelay.hpp"
%}

%include "groverotary.hpp"
%{
    #include "groverotary.hpp"
%}

%include "grovetemp.hpp"
%{
    #include "grovetemp.hpp"
%}
