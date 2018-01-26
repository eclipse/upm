#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::ICO2Sensor);
#endif
%include "stdint.i"
%include "interfaces.i"
%include "javaupm_iModuleStatus.i"

%include "iCO2Sensor.hpp"
%{
	#include "iCO2Sensor.hpp"
%}
