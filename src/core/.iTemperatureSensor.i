%include ".iSensorType.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iTemperatureSensor);
#endif
#endif

%{
#include "iTemperatureSensor.hpp"
%}
%include "iTemperatureSensor.hpp"

/* FOR JAVASCRIPT ONLY!
If there's a better way to allow multiple inheritance w/javascript, the
following macro definitions can go away.
    usage: INHERIT_ITEMPERATURESENSOR(upm::MySensorClass)
*/
%define INHERIT_ITEMPERATURESENSOR(YourClassName)
%extend YourClassName {
/*using upm::iSensorType::JsonDefinition;*/
/*using upm::iSensorType::JsonValues;*/
using upm::iTemperatureSensor::JsonTemperature;
using upm::iTemperatureSensor::TemperatureAll;
using upm::iTemperatureSensor::TemperatureForSources;
}
%enddef
