#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IADC);
#endif
%include "interfaces.i"
%include "javaupm_iModuleStatus.i"

%include "iADC.hpp"
%{
	#include "iADC.hpp"
%}
