%module javaupm_adxl335
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"

%apply int *OUTPUT { int *xVal, int *yVal, int *zVal };
%apply float *OUTPUT { float *xAccel, float *yAccel, float *zAccel  };

%{
    #include "adxl335.h"
%}

%typemap(jni) float* "jfloatArray"
%typemap(jstype) float* "float[]"
%typemap(jtype) float* "float[]"

%typemap(javaout) float* {
    return $jnicall;
}

%typemap(out) float *acceleration {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(jni) int* "jintArray"
%typemap(jstype) int* "int[]"
%typemap(jtype) int* "int[]"

%typemap(javaout) int* {
    return $jnicall;
}

%typemap(out) int *values {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const signed int*)$1);
    delete [] $1;
}

%ignore values(int *, int *, int *);
%ignore acceleration(float *, float *, float *);

%include "adxl335.h"
