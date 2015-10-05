%module(directors="1") javaupm_mma7660
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%apply int *OUTPUT { int *x, int *y, int *z };
%apply float *OUTPUT { float *ax, float *ay, float *az  };

%{
    #include "mma7660.h"
%}

%typemap(jni) float* "jfloatArray"
%typemap(jstype) float* "float[]"
%typemap(jtype) float* "float[]"

%typemap(javaout) float* {
    return $jnicall;
}

%typemap(out) float *getAcceleration {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
    delete [] $1;
}


%typemap(jni) int* "jintArray"
%typemap(jstype) int* "int[]"
%typemap(jtype) int* "int[]"

%typemap(javaout) int* {
    return $jnicall;
}

%typemap(out) int *getRawValues {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const signed int*)$1);
    delete [] $1;
}

%ignore getRawValues(int *, int *, int *);
%ignore getAcceleration(float *, float *, float *);

%include "mma7660.h"
