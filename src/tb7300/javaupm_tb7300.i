%module javaupm_tb7300
%include "../upm.i"
%include "typemaps.i"

// We need to use this method for enum wrapping since the enum typedefs used
// by the derived classes (like ANALOG_VALUES_T) are passed to methods
// in the base class which expect a uint32_t.  This is fine, and
// works everywhere except Java.  It's type safety is a little too
// stringent in these cases.  %javaconst(1) is generally recommended
// as it avoids JNI calls to determine the enumerant values at
// runtime.
%include "enumtypeunsafe.swg"
%javaconst(1);

%include "bacnetmstp.hpp"
%include "bacnetutil.hpp"
%include "tb7300.hpp"
%{
    #include "tb7300.hpp"
%}


JAVA_JNI_LOADLIBRARY(javaupm_tb7300)
