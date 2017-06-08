%include ".iUpmObject.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface(upm::iActuatorType);
#endif
%ignore upm::iUpmObject::JsonDefinition;
#endif

%{
#include "iActuatorType.hpp"
%}
%include "iActuatorType.hpp"
