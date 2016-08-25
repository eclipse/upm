%module javaupm_bmx055
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
}

%typemap(out) float *getGyroscope {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(out) float *getMagnetometer {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);

%include "bma250e.hpp"
%{
    #include "bma250e.hpp"
%}

%include "bmg160.hpp"
%{
    #include "bmg160.hpp"
%}

%include "bmm150.hpp"
%{
    #include "bmm150.hpp"
%}

%include "bmx055.hpp"
%{
    #include "bmx055.hpp"
%}

%include "bmc150.hpp"
%{
    #include "bmc150.hpp"
%}

%include "bmi055.hpp"
%{
    #include "bmi055.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmx055");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
