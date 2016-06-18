%module javaupm_h3lis331dl
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"

%apply int *OUTPUT { int *x, int *y, int*z };
%apply float *OUTPUT { float *aX, float *aY, float *aZ  };

%ignore i2cContext;

%{
    #include "h3lis331dl.hpp"
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

%typemap(out) int *getRawXYZ {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const int*)$1);
}

%typemap(out) int *getXYZ {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const int*)$1);
}

%ignore getRawXYZ(int *, int *, int *);
%ignore getXYZ(int *, int *, int *);
%ignore getAcceleration(float *, float *, float *);

%include "h3lis331dl.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_h3lis331dl");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}