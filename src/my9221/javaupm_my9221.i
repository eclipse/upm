%module javaupm_my9221
%include "../upm.i"

%include "my9221.h"
%{
    #include "my9221.h"
%}

%include "groveledbar.h"
%{
    #include "groveledbar.h"
%}

%include "grovecircularled.h"
%{
    #include "grovecircularled.h"
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
