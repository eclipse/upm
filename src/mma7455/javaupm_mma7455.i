%module javaupm_mma7455
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i";

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

%typemap(jni) (unsigned char *buf, unsigned char size) "jbyteArray";
%typemap(jtype) (unsigned char *buf, unsigned char size) "byte[]";
%typemap(jstype) (unsigned char *buf, unsigned char size) "byte[]";

%typemap(javain) (unsigned char *buf, unsigned char size) "$javainput";

%typemap(in) (unsigned char *buf, unsigned char size) {
        $1 = (unsigned char *) JCALL2(GetByteArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (unsigned char *buf, unsigned char size) {
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *)$1, 0);
}

%include "mma7455.h"
