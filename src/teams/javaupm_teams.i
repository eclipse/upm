%module javaupm_teams
%include "../upm.i"
%include "std_string.i"

%include "teams.hpp"
%{
    #include "teams.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_teams)
