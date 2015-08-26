%module pyupm_lsm303
%include "../upm.i"
%include "../carrays_int16_t.i"

%feature("autodoc", "3");

// Adding this typemap because SWIG is converting int16 into a short by default
// This forces SWIG to convert it correctly
%typemap(out) int16_t* {
	resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 |  0 );
}

%include "lsm303.h"
%{
    #include "lsm303.h"
%}
