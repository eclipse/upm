%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"
%apply unsigned char *OUTPUT { uint8_t *val };

JAVA_JNI_LOADLIBRARY(javaupm_ozw)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(float, floatp);

%{
#include "ozwinterface.hpp"
#include "ozw.hpp"
#include "aeotecdsb09104.hpp"
#include "aeotecdw2e.hpp"
#include "aeotecsdg2.hpp"
#include "aeotecss6.hpp"
#include "ozwdump.hpp"
#include "tzemt400.hpp"
%}
%include "ozwinterface.hpp"
%include "ozw.hpp"
%include "aeotecdsb09104.hpp"
%include "aeotecdw2e.hpp"
%include "aeotecsdg2.hpp"
%include "aeotecss6.hpp"
%include "ozwdump.hpp"
%include "tzemt400.hpp"
/* END Common SWIG syntax */
