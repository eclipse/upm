%module javaupm_bno055
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%apply int {mraa::Edge};
%apply float *INOUT { float *x, float *y, float *z };
%apply float *INOUT { float *heading, float *roll, float *pitch };

%typemap(jni) float* "jfloatArray"
%typemap(jstype) float* "float[]"
%typemap(jtype) float* "float[]"

%typemap(javaout) float* {
    return $jnicall;
}

%typemap(jni) int* "jintArray"
%typemap(jstype) int* "int[]"
%typemap(jtype) int* "int[]"

%typemap(javaout) int* {
    return $jnicall;
}


%typemap(out) float *getAccelerometer {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(out) float *getMagnetometer {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(out) float *getGyroscope {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(out) float *getEulerAngles {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(out) float *getQuaternions {
    $result = JCALL1(NewFloatArray, jenv, 4);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 4, $1);
}

%typemap(out) float *getLinearAcceleration {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(out) float *getGravityVectors {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(out) int *getCalibrationStatus {
    $result = JCALL1(NewIntArray, jenv, 4);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 4, (const int*)$1);
}

%ignore getCalibrationStatus(int *, int *, int *, int *);
%ignore getAccelerometer(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getEulerAngles(float *, float *, float *);
%ignore getQuaternions(float *, float *, float *, float *);
%ignore getLinearAcceleration(float *, float *, float *);
%ignore getGravityVectors(float *, float *, float *);

%{
    #include "bno055.hpp"
%}

%include "bno055.hpp"

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
