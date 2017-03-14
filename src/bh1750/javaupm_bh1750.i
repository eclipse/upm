%module javaupm_bh1750
%include "../upm.i"
%include "std_string.i"
%include "enums.swg"

%{
    #include "bh1750.hpp"
%}

%include "bh1750.hpp"

%javaconst(1);
enum BH1750_OPMODES_T {
  BH1750_OPMODE_H1_CONT, // continuous 1 lx high resolution
  BH1750_OPMODE_H2_CONT, // continuous .5 lx high resolution
  BH1750_OPMODE_L_CONT,  // continuous 4 lx low resolution
  BH1750_OPMODE_H1_ONCE, // onetime 1 lx high resolution
  BH1750_OPMODE_H2_ONCE, // onetime .5 lx high resolution
  BH1750_OPMODE_L_ONCE,  // onetime 4 lx low resolution
};

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bh1750");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
