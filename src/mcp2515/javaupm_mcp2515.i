%module javaupm_mcp2515
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "../java_buffer.i"
%include "std_string.i"

%include "mcp2515_regs.h"
%include "mcp2515.hpp"
%{
    #include "mcp2515.hpp"
%}


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mcp2515");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
