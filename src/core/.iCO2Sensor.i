%include ".iSensorType.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iCO2Sensor);
#endif
#endif

%{
#include "iCO2Sensor.hpp"
%}
%include "iCO2Sensor.hpp"
