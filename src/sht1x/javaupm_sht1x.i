%module javaupm_sht1x
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"
%include "enums.swg"

%include "sht1x.hpp"
%{
    #include "sht1x.hpp"
%}

%javaconst(1);
enum SHT1X_VOLTS_T {
	SHT1X_VOLTS_5 		= 0, // 5 volts
    SHT1X_VOLTS_4       = 1,
    SHT1X_VOLTS_3_5     = 2, // 3.5v
    SHT1X_VOLTS_3       = 3,
    SHT1X_VOLTS_2_5     = 4
};

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_sht1x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
