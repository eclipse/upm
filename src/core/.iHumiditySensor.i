%include ".iSensorType.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iHumiditySensor);
#endif
#endif

%{
#include "iHumiditySensor.hpp"
%}
%include "iHumiditySensor.hpp"


/* FOR JAVASCRIPT ONLY!
If there's a better way to allow multiple inheritance w/javascript, the
following macro definitions can go away.
    usage: INHERIT_IHUMIDITYSENSOR(upm::MySensorClass)
*/
%define INHERIT_IHUMIDITYSENSOR(YourClassName)
%extend YourClassName {
/*using upm::iSensorType::JsonDefinition;*/
/*using upm::iSensorType::JsonValues;*/
using upm::iHumiditySensor::JsonHumidity;
using upm::iHumiditySensor::HumidityAll;
using upm::iHumiditySensor::HumidityForSources;
}
%enddef
