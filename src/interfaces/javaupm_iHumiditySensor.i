#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IHumiditySensor);
#endif

%include "javaupm_interfaces.i"
%include "javaupm_iModuleStatus.i"

%include "iHumiditySensor.hpp"
%{
	#include "iHumiditySensor.hpp"
%}