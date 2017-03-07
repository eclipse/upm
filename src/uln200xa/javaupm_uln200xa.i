%module javaupm_uln200xa
%include "../upm.i"
%include "enums.swg"

%javaconst(1);
enum ULN200XA_DIRECTION_T {
	ULN200XA_DIR_CW   = 0x01,
    ULN200XA_DIR_CCW  = 0x02
};

%include "uln200xa.hpp"
%{
    #include "uln200xa.hpp"
%}


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_uln200xa");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
