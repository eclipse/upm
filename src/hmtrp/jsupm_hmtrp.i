%module jsupm_hmtrp
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"

// Adding this typemap because SWIG is converting uint8, uint16, and uint32 into a short by default
// This forces SWIG to convert it correctly
%typemap(in) uint8_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp, SWIGTYPE_p_uint8Array, 0 |  0 );
  $1 = (uint8_t *)(argp);
}

%typemap(in) uint16_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp, SWIGTYPE_p_uint16Array, 0 |  0 );
  $1 = (uint16_t *)(argp);
}

%typemap(in) uint32_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp, SWIGTYPE_p_uint32Array, 0 |  0 );
  $1 = (uint32_t *)(argp);
}

%{
    #include "hmtrp.h"
    speed_t int_B9600 = B9600;
%}

%include "hmtrp.h"
speed_t int_B9600 = B9600;
%array_class(char, charArray);
