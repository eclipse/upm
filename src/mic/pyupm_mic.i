%module pyupm_mic
%include "../upm.i"
%include "../carrays_uint16_t.i"

%typemap(in) uint16_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint16Array, 0 |  0 );
  $1 = reinterpret_cast< uint16_t * >(argp);
}

%{
    #include "mic.h"
%}
%include "mic.h"
