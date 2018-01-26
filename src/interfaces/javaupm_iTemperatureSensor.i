#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::ITemperatureSensor);
#endif
%include "interfaces.i"
%include "javaupm_iModuleStatus.i"

%include "iTemperatureSensor.hpp"
%{
	#include "iTemperatureSensor.hpp"
%}
