%module javaupm_teams
%include "../upm.i"
%include "std_string.i"

%include "teams.hpp"
%{
    #include "teams.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_teams");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
