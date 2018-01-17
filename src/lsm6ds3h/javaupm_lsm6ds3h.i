%module javaupm_lsm6ds3h
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%{
    #include "lsm6ds3h.hpp"
    #include "lsm6ds3h_defs.h"
%}
%include "lsm6ds3h_defs.h"
%include "lsm6ds3h.hpp"

%ignore installISR(LSM6DS3H_INTERRUPT_PINS_T , int ,  mraa::Edge ,  void *, void *);

%extend upm::LSM6DS3H {
    void installISR(LSM6DS3H_INTERRUPT_PINS_T intr, int gpio,
                    mraa::Edge level, jobject runnable)
        {
            $self->installISR(intr, gpio, level, mraa_java_isr_callback,
                       runnable);
        }
}

JAVA_JNI_LOADLIBRARY(javaupm_lsm6ds3h)
