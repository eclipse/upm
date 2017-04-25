#if SWIG_VERSION >= 0x030009
    %include <swiginterface.i>
    %interface_impl(upm::IStepper);
#endif

%include "javaupm_interfaces.i"
%include "javaupm_iUPMObject.i"

%include "iStepper.hpp"
