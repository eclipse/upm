// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_teams
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%{
    #include "teams.h"
%}
%include "teams.h"

