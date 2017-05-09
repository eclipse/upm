#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IGyroscope);
#endif

%include "javaupm_interfaces.i"
%include "javaupm_iUPMObject.i"

%include "iGyroscope.hpp"
%{
	#include "iGyroscope.hpp"
%}
