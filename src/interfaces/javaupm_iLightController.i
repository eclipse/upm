#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::ILightController);
#endif
%include "interfaces.i"
%include "javaupm_iModuleStatus.i"

%{
#include "iLightController.hpp"
%}
%include "iLightController.hpp"
