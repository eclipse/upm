%module (docstring="Basic Grove sensors") javaupm_grove

%include "../upm.i"

%apply int {mraa::Edge}

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

%include "grovelight.hpp"
%{
    #include "grovelight.hpp"
%}

%include "groverelay.hpp"
%{
    #include "groverelay.hpp"
%}

%include "groverotary.hpp"
%{
    #include "groverotary.hpp"
%}

%include "groveslide.hpp"
%{
    #include "groveslide.hpp"
%}

%include "grovetemp.hpp"
%{
    #include "grovetemp.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grove");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
