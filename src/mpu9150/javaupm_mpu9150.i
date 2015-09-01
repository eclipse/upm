%module javaupm_mpu9150
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"

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

%typemap(jni) (uint8_t *buf, int len) "jbyteArray";
%typemap(jtype) (uint8_t *buf, int len) "byte[]";
%typemap(jstype) (uint8_t *buf, int len) "byte[]";

%typemap(javain) (uint8_t *buf, int len) "$javainput";

%typemap(in) (uint8_t *buf, int len) {
        $1 = (uint8_t *) JCALL2(GetByteArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint8_t *buf, int len) {
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *)$1, 0);
}

%include "mpu60x0.h"
%include "mpu9150.h"
