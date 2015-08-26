%module javaupm_mma7455
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i";

%apply unsigned char[] {unsigned char *};
%apply short *OUTPUT { short * ptrX, short * ptrY, short * ptrZ };

%{
    #include "mma7455.h"
%}

%typemap(jni) short* "jshortArray"
%typemap(jstype) short* "short[]"
%typemap(jtype) short* "short[]"

%typemap(javaout) short* {
    return $jnicall;
}

%typemap(out) short *readData {
    $result = JCALL1(NewShortArray, jenv, 3);
    JCALL4(SetShortArrayRegion, jenv, $result, 0, 3, (const signed short*)$1);
    delete [] $1;
}

%ignore readData(short *, short *, short *);

%include "mma7455.h"
