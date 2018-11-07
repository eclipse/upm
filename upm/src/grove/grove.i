%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%apply int {mraa::Edge}

JAVA_ADD_INSTALLISR_EDGE(upm::GroveButton)
JAVA_JNI_LOADLIBRARY(javaupm_grove)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "grovebase.hpp"
#include "grove.hpp"
#include "grovebutton.hpp"
#include "groveled.hpp"
#include "grovelight.hpp"
#include "groverelay.hpp"
#include "groverotary.hpp"
#include "groveslide.hpp"
#include "grovetemp.hpp"
%}
%include "grovebase.hpp"
%include "grove.hpp"
%include "grovebutton.hpp"
%include "groveled.hpp"
%include "grovelight.hpp"
%include "groverelay.hpp"
%include "groverotary.hpp"
%include "groveslide.hpp"
%include "grovetemp.hpp"
/* END Common SWIG syntax */
