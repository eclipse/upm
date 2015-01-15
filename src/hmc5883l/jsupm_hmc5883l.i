%module jsupm_hmc5883l
%include "../upm.i"
%include "../carrays_int16_t.i"
%{
    #include "hmc5883l.h"
%}

// Adding this typemap because SWIG is converting int16 into a short by default
// This forces SWIG to convert it correctly
%typemap(out) int16_t* {
  $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 |  0 );
}

%include "hmc5883l.h"
