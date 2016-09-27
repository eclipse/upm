%module javaupm_bmi160
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "../java_buffer.i"
%include "std_string.i"

%{
    #include "bmi160.hpp"
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
}

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);

%include "bmi160.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmi160");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
