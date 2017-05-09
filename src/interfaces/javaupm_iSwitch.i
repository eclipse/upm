#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::ISwitch);
#endif

%include "javaupm_interfaces.i"
%include "javaupm_iUPMObject.i"

%include "iSwitch.hpp"
%{
	#include "iSwitch.hpp"
%}
