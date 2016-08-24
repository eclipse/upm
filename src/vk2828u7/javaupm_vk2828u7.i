%module javaupm_vk2828u7
%include "../upm.i"
%include "std_string.i"
%include "typemaps.i"

%include "vk2828u7.hpp"
%{
    #include "vk2828u7.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_vk2828u7");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
