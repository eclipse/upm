%module javaupm_st7735
%include "../upm.i"
%include "arrays_java.i"
%include "typemaps.i"
%include "stdint.i"

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

%include "gfx.h"
%{
    #include "gfx.h"
%}

%include "st7735.h"
%{
    #include "st7735.h"
%}
