%module javaupm_adc121c021
%include "../upm.i"
/*%include "../carrays_uint16_t.i"
*/
%{
    #include "adc121c021.hpp"
%}

%include "adc121c021.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_adc121c021");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}