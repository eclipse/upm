// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_zfm20
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"
%include "cpointer.i"

%feature("autodoc", "3");

/* Send "int *" to python as intp */
%pointer_functions(int, intp);
%{
    #include "zfm20.hpp"
    speed_t int_B57600 = B57600;
%}
%include "zfm20.hpp"
speed_t int_B57600 = B57600;
