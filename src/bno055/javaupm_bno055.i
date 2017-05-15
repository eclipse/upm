%module javaupm_bno055
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "std_vector.i"
%include "stdint.i"

%ignore getCalibrationStatus(int *, int *, int *, int *);
%ignore getAccelerometer(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getEulerAngles(float *, float *, float *);
%ignore getQuaternions(float *, float *, float *, float *);
%ignore getLinearAcceleration(float *, float *, float *);
%ignore getGravityVectors(float *, float *, float *);

%template(floatVector) std::vector<float>;
%template(byteVector) std::vector<uint8_t>;
%template(intVector) std::vector<int>;

%include "bno055_regs.h"
%include "bno055.hpp"
%{
    #include "bno055.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bno055");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
