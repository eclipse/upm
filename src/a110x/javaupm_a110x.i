%module javaupm_a110x
%include "stdint.i"
%include "typemaps.i"

%include "common.i"

/* Macro for loading javaupm_a110x */
JAVA_JNI_LOADLIBRARY(javaupm_a110x)

/* Macro adding an installisr call w/JAVA runnable to A110X */
JAVA_ADD_INSTALLISR(upm::A110X)
