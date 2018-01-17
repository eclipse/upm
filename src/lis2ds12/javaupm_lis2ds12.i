%module javaupm_lis2ds12
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);

%{
    #include "lis2ds12.hpp"
    #include "lis2ds12_defs.h"
%}
%include "lis2ds12_defs.h"
%include "lis2ds12.hpp"


#%ignore installISR(LIS2DS12_INTERRUPT_PINS_T , int ,  mraa::Edge, void *, void *);

%extend upm::LIS2DS12 {
    void installISR(LIS2DS12_INTERRUPT_PINS_T intr, int gpio,
                    mraa::Edge level, jobject runnable)
        {
            $self->installISR(intr, gpio, level, mraa_java_isr_callback,
                       runnable);
        }
}

JAVA_JNI_LOADLIBRARY(javaupm_lis2ds12)
