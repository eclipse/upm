%include ".iSensorType.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iADC);
#endif
#endif

%{
#include "iADC.hpp"
%}
%include "iADC.hpp"
