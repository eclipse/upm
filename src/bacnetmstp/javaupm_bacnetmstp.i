%module javaupm_bacnetmstp
%include "../upm.i"
%include "typemaps.i"
%include "carrays_uint32_t.i"

%{
    #include "bacnetmstp.hpp"
    #include "bacnetutil.hpp"
%}

%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bacnetmstp");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
