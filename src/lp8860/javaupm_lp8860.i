%module javaupm_lp8860

%import "../interfaces/javaupm_iLightController.i"

%include "../upm.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "lp8860.hpp"
%}

%include "lp8860.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lp8860");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
