#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IAngle);
#endif

%include "javaupm_interfaces.i"
%include "javaupm_iUPMObject.i"

%include "iAngle.hpp"
%{
	#include "iAngle.hpp"
%}
