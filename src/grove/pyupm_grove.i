// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grove
%include "../upm.i"

%feature("autodoc", "3");

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

%include "relay.hpp"
%{
    #include "relay.hpp"
%}

%include "groverotary.hpp"
%{
    #include "groverotary.hpp"
%}

%include "grovetemp.hpp"
%{
    #include "grovetemp.hpp"
%}
