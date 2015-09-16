%module (directors="1", docstring="Basic Grove sensors") javaupm_grove

%include "../upm.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%apply int {mraa::Edge}

%{
    #include "grove.h"
%}
/*
%typemap(jni) mraa_result_t "jint"
%typemap(jstype) mraa_result_t "int"
%typemap(jtype) mraa_result_t "int"


%typemap(javaout) mraa_result_t {
    return $jnicall;
}


%typemap(out) mraa_result_t {
        $result = (int)$1;
}
*/
%include "grove.h"

