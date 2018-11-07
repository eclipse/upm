%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_buzzer)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "buzzer.hpp"
%}
%include "buzzer_tones.h"
%include "buzzer.hpp"
/* END Common SWIG syntax */
