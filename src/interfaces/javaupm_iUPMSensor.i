#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IUPMSensor);
#endif

%include "javaupm_interfaces.i"
%include "javaupm_iUPMObject.i"

%include "iUPMSensor.hpp"
%{
	#include "iUPMSensor.hpp"
%}
