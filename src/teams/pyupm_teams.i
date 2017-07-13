// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_teams
%include "../upm.i"
%include "std_string.i"

%{
    #include "teams.hpp"
%}
%include "teams.hpp"

