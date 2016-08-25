// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hmtrp
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"

%rename("getModSignalStrengthNoParam")  getModSignalStrength();
%rename("getRFSignalStrengthNoParam") getRFSignalStrength();

%feature("autodoc", "3");

%include "hmtrp.hpp"
%{
    #include "hmtrp.hpp"
    speed_t int_B9600 = B9600;
%}
speed_t int_B9600 = B9600;
%array_class(char, charArray);
