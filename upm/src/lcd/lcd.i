%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";

%apply signed char[] {uint8_t []};
%ignore BasicFont;

%typemap(jni) (uint8_t *data, int bytes) "jbyteArray";
%typemap(jtype) (uint8_t *data, int bytes) "byte[]";
%typemap(jstype) (uint8_t *data, int bytes) "byte[]";

%typemap(javain) (uint8_t *data, int bytes) "$javainput";

%typemap(in) (uint8_t *data, int bytes) {
        $1 = (uint8_t *) JCALL2(GetByteArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint8_t *data, int bytes) {
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *)$1, 0);
}

JAVA_JNI_LOADLIBRARY(javaupm_i2clcd)
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
#include "lcd.hpp"
#include "eboled.hpp"
#include "ssd1306.hpp"
#include "ssd1308.hpp"
#include "ssd1327.hpp"
%}

%include "lcd.hpp"
%include "eboled.hpp"
%include "ssd1306.hpp"
%include "ssd1308.hpp"
%include "ssd1327.hpp"
%include "ssd.hpp"
/* END Common SWIG syntax */
