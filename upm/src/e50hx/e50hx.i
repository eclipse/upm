%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../carrays_uint32_t.i"

// We need to use this method for enum wrapping since the enum typedefs used
// by the derived classes (like ANALOG_VALUES_T) are passed to methods
// in the base class which expect a uint32_t.  This is fine, and
// works everywhere except Java.  Its type safety is a little too
// stringent in these cases.  %javaconst(1) is generally recommended
// as it avoids JNI calls to determine the enumerant values at
// runtime.
%include "enumtypeunsafe.swg"
%javaconst(1);

JAVA_JNI_LOADLIBRARY(javaupm_e50hx)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bacnetmstp.hpp"
#include "bacnetutil.hpp"
#include "e50hx.hpp"
%}
%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"
%include "e50hx.hpp"
/* END Common SWIG syntax */
