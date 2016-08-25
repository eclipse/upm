%module javaupm_ozw
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

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

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ozw");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
