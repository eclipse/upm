%module jsupm_itg3200
%include "../upm.i"
%include "../carrays_int16_t.i"
%include "../carrays_float.i"

%{
    #include "itg3200.h"
%}

%typemap(out) int16_t * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 | 0 );
}

%typemap(out) float * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_floatArray, 0 | 0 );
}

%include "itg3200.h"

%include <carrays.i>
