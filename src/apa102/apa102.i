%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(jtype) (uint8_t *colors) "byte[]"
%typemap(jstype) (uint8_t *colors) "byte[]"
%typemap(jni) (uint8_t *colors) "jbyteArray"
%typemap(javain) (uint8_t *colors) "$javainput"

%typemap(in) (uint8_t *colors) {
  $1 = (uint8_t*)JCALL2(GetByteArrayElements, jenv, $input, NULL);
}

JAVA_JNI_LOADLIBRARY(javaupm_apa102)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%inline %{
    #include <node_buffer.h>
%}

%typemap(in) (uint8_t *colors) {
  if (!node::Buffer::HasInstance($input)) {
      SWIG_exception_fail(SWIG_ERROR, "Expected a node Buffer");
  }
  $1 = (uint8_t*) node::Buffer::Data($input);
}
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
// setLeds
%typemap(in) (uint8_t *colors) {
  if (PyByteArray_Check($input)) {
    $1 = (uint8_t*) PyByteArray_AsString($input);
  } else {
    PyErr_SetString(PyExc_ValueError, "bytearray expected");
    return NULL;
  }
}
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "apa102.hpp"
%}
%include "apa102.hpp"
/* END Common SWIG syntax */
