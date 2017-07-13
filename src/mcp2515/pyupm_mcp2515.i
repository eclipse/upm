// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mcp2515
%include "../upm.i"
%include "cpointer.i"
%include "std_string.i"
%include "../carrays_uint8_t.i"

%include "stdint.i"

%pointer_functions(float, floatp);

%include "mcp2515_regs.h"
%include "mcp2515.hpp"
%{
    #include "mcp2515.hpp"
%}
