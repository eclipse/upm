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

%{
    #include "bno055.hpp"
    #include "bno055_regs.h"
%}
%include "bno055_regs.h"
%include "bno055.hpp"

%ignore installISR(int , mraa::Edge ,  void *, void *);

%extend upm::BNO055 {
    void installISR(int gpio, mraa_gpio_edge_t level, jobject runnable)
    {
        $self->installISR(gpio, level, mraa_java_isr_callback, runnable);
    }
}

JAVA_JNI_LOADLIBRARY(javaupm_bno055)
