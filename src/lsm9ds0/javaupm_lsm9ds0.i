%module javaupm_lsm9ds0
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%apply int {mraa::Edge};
%apply float *INOUT { float *x, float *y, float *z };

%typemap(jni) float* "jfloatArray"
%typemap(jstype) float* "float[]"
%typemap(jtype) float* "float[]"

%typemap(javaout) float* {
    return $jnicall;
}

%typemap(out) float *getAccelerometer {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
    delete [] $1;
}

%typemap(out) float *getGyroscope {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
    delete [] $1;
}

%typemap(out) float *getMagnetometer {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
    delete [] $1;
}

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);

%{
    #include "lsm9ds0.hpp"
%}

%ignore installISR(LSM6DSL_INTERRUPT_PINS_T , int ,  mraa::Edge ,  void *, void *);

%extend upm::LSM9DS0 {
    installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
    			 jobject runnable)
    {
      // delete any existing ISR and GPIO context
      $self->uninstallISR(intr);

      // greate gpio context
      getPin(intr) = new mraa::Gpio(gpio);

      getPin(intr)->dir(mraa::DIR_IN);
      getPin(intr)->isr(level, runnable);

    }
}

%include "lsm9ds0.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_lsm9ds0)
