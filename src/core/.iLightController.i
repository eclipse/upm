%include ".iSensorType.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iLightController);
#endif
#endif

%{
#include "iLightController.hpp"
%}
%include "iLightController.hpp"
