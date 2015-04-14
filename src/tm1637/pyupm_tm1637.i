%module pyupm_tm1637
%include "../upm.i"
%include "../carrays_uint8_t.i"

%varargs(4, int digit = 0) write;

%typemap(in) uint8_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint8Array, 0 |  0 );
  $1 = reinterpret_cast< uint8_t * >(argp);
}

%{
    #include "tm1637.h"
%}
%include "tm1637.h"
