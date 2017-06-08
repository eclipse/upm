%include ".iSensorType.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iMoistureSensor);
#endif
#endif

%{
#include "iMoistureSensor.hpp"
%}
%include "iMoistureSensor.hpp"
