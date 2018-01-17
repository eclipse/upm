%module javaupm_mpu9150
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "../java_buffer.i"

%apply int {mraa::Edge};

%{
    #include "mpu60x0.hpp"
    #include "mpu9150.hpp"
%}


%typemap(jni) float * "jfloatArray"
%typemap(jstype) float * "float[]"
%typemap(jtype) float * "float[]"

%typemap(javaout) float * {
    return $jnicall;
}

%typemap(out) float * {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
    delete [] $1;
}

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);

%include "mpu60x0.hpp"
%include "mpu9150.hpp"

%ignore installISR(int , mraa::Edge , void *, void *);

%extend upm::MPU60X0 {
    void installISR(int gpio, mraa::Edge level,
                             jobject runnable)
    {
      // delete any existing ISR and GPIO context
      $self->uninstallISR();

      // greate gpio context
      mraa::Gpio* swg_gpioIRQ = $self->get_gpioIRQ();

      swg_gpioIRQ->dir(mraa::DIR_IN);
      swg_gpioIRQ->isr(level, runnable);
    }
}

JAVA_JNI_LOADLIBRARY(javaupm_mpu9150)
