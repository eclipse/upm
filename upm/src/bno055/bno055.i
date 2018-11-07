%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "std_vector.i"
%ignore getCalibrationStatus(int *, int *, int *, int *);
%ignore getAccelerometer(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getEulerAngles(float *, float *, float *);
%ignore getQuaternions(float *, float *, float *, float *);
%ignore getLinearAcceleration(float *, float *, float *);
%ignore getGravityVectors(float *, float *, float *);
%ignore installISR(int , mraa::Edge ,  void *, void *);

%template(floatVector) std::vector<float>;
%template(byteVector) std::vector<uint8_t>;
%template(intVector) std::vector<int>;

%extend upm::BNO055 {
    void installISR(int gpio, mraa_gpio_edge_t level, jobject runnable)
    {
        $self->installISR(gpio, level, mraa_java_isr_callback, runnable);
    }
}

JAVA_JNI_LOADLIBRARY(javaupm_bno055)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../upm_vectortypes.i"
%pointer_functions(int, intp);
%pointer_functions(float, floatp);
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../upm_vectortypes.i"
%pointer_functions(int, intp);
%pointer_functions(float, floatp);
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bno055.hpp"
%}
%include "bno055_regs.h"
%include "bno055.hpp"
/* END Common SWIG syntax */
