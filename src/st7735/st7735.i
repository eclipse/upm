%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i"
%ignore m_map;
%ignore Bcmd;
%ignore font;
%ignore Rcmd1;
%ignore Rcmd2green;
%ignore Rcmd2red;
%ignore Rcmd3;

/*
%typemap(jni) unsigned char[] "jbyteArray";
%typemap(jtype) unsigned char[] "byte[]";
%typemap(jstype) unsigned char[] "byte[]";

%typemap(out) unsigned char[] {
    $result = SWIG_JavaArrayOutSchar(jenv, reinterpret_cast<signed char*>($1), sizeof($1));
}
%typemap(freearg) unsigned char* {
    JCALL3(ReleaseByteArrayElements, jenv, $input, reinterpret_cast<jbyte*>($1), 0);
}
*/
%typemap(jni) unsigned char* "jbyteArray";
%typemap(jtype) unsigned char* "byte[]";
%typemap(jstype) unsigned char* "byte[]";

%typemap(javain) unsigned char* "$javainput"

%typemap(in) unsigned char* {
    $1 = (unsigned char *)JCALL2(GetByteArrayElements, jenv, $input, NULL);
}

JAVA_JNI_LOADLIBRARY(javaupm_st7735)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_uint8_t.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../carrays_uint8_t.i"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "st7735_gfx.hpp"
#include "st7735.hpp"
%}
%include "st7735_gfx.hpp"
%include "st7735.hpp"
/* END Common SWIG syntax */
