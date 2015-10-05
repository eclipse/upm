%module(directors="1") javaupm_mpu9150
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "../java_buffer.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%apply int {mraa::Edge};

%{
    #include "mpu60x0.h"
    #include "mpu9150.h"
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

%include "mpu60x0.h"
%include "mpu9150.h"
