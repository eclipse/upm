%include ".iSensorType.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iPressureSensor);
#endif
#endif

%{
#include "iPressureSensor.hpp"
%}
%include "iPressureSensor.hpp"
