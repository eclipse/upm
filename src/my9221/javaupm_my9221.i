%module javaupm_my9221
%include "../upm.i"

%include "my9221.hpp"
%{
    #include "my9221.hpp"
%}

%include "groveledbar.hpp"
%{
    #include "groveledbar.hpp"
%}

%include "grovecircularled.hpp"
%{
    #include "grovecircularled.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_my9221");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
