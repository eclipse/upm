%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%typemap(jni) unsigned char* "jbyteArray";
%typemap(jtype) unsigned char* "byte[]";
%typemap(jstype) unsigned char* "byte[]";

%typemap(javaout) unsigned char* {
 return $jnicall;
}

%typemap(out) unsigned char* {
    $result = JCALL1(NewByteArray, jenv, LOL_X*LOL_Y);
    JCALL4(SetByteArrayRegion, jenv, $result, 0, LOL_X*LOL_Y-1, reinterpret_cast<jbyte*>($1));
    delete [] $1;
}

JAVA_JNI_LOADLIBRARY(javaupm_lol)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "lol.hpp"
%}
%include "lol.hpp"
/* END Common SWIG syntax */
