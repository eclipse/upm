%module javaupm_lcdks
%include "../upm.i"
%include "typemaps.i"
%include "std_vector.i"
%include "stdint.i"

%template(byteVector) std::vector<uint8_t>;

%include "lcdks.hpp"
%{
    #include "lcdks.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lcdks");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
