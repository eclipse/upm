%module pyupm_wt5001
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"

%typemap(in) uint8_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint8Array, 0 |  0 );
  $1 = reinterpret_cast< uint8_t * >(argp);
}

%typemap(in) uint16_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint16Array, 0 |  0 );
  $1 = reinterpret_cast< uint16_t * >(argp);
}

%feature("autodoc", "3");

%{
    #include "wt5001.h"
    speed_t int_B9600 = B9600;
%}
%include "wt5001.h"
speed_t int_B9600 = B9600;
