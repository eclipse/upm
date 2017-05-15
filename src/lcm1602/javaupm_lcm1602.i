%module javaupm_lcm1602
%include "../upm.i"
%include "typemaps.i"
%include "std_vector.i"
%include "stdint.i"

%template(byteVector) std::vector<uint8_t>;

%include "lcm1602.hpp"
%{
    #include "lcm1602.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lcm1602");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
