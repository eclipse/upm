#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IModuleStatus);
#endif
%include "iModuleStatus.hpp"
%{
	#include "iModuleStatus.hpp"
%}
