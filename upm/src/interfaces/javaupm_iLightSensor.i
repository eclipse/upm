#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::ILightSensor);
#endif
%include "interfaces.i"
%include "javaupm_iModuleStatus.i"

%include "iLightSensor.hpp"
%{
	#include "iLightSensor.hpp"
%}
