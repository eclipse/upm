%include ".iUpmObject.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface(upm::iSensorType);
#endif
%ignore upm::iUpmObject::JsonDefinition;
#endif

%{
#include "iSensorType.hpp"
%}
%include "iSensorType.hpp"
