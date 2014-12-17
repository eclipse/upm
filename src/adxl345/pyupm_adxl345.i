%module pyupm_adxl345
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
%include "adxl345_doc.i"
#endif

%include "adxl345.h"
%{
    #include "adxl345.h"
%}
