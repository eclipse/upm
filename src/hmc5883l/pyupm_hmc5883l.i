%module pyupm_hmc5883l
%include "../upm.i"
%include "../carrays_int16_t.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "hmc5883l_doc.i"
#endif

%typemap(out) int16_t* {
  $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 |  0 );
}

%include "hmc5883l.h"
%{
    #include "hmc5883l.h"
%}
