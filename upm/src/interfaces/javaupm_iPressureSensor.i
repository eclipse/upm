#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IPressureSensor);
#endif
%include "interfaces.i"
%include "javaupm_iModuleStatus.i"

%include "iPressureSensor.hpp"
%{
	#include "iPressureSensor.hpp"
%}
