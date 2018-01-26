%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
/* Add the JAVA code to load the module */
JAVA_JNI_LOADLIBRARY(javaupm_sensortemplate)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
/* SWIG preprocessor copies this directly to the output wrapper*/
#include "sensortemplate.hpp"
%}

/* %include is a SWIG directive.  SWIG will wrap types exposed in this
 * header to the target language. */
%include "sensortemplate.hpp"
/* END Common SWIG syntax */
