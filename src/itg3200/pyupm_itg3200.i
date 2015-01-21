%module pyupm_itg3200
%include "../upm.i"
%include "../carrays_int16_t.i"
%include "../carrays_float.i"

%typemap(out) int16_t * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 | 0 );
}

%typemap(out) float * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_floatArray, 0 | 0 );
}

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "itg3200_doc.i"
#endif

%include "itg3200.h"
%{
    #include "itg3200.h"
%}
