%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_bh1749)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "std_vector.i"
%template(uint16Vector) std::vector<uint16_t>;

%{
#include "bh1749.hpp"
%}
%include "bh1749.h"
%include "bh1749.hpp"
/* END Common SWIG syntax */
