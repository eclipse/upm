%module javaupm_t8100
%include "../upm.i"
%include "typemaps.i"

%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"
%include "t8100.hpp"
%{
    #include "t8100.hpp"
%}


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_t8100");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
