%module pyupm_zfm20
%include "../upm.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"
%include "cpointer.i"

%feature("autodoc", "3");

%typemap(in) uint16_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint16Array, 0 |  0 );
  $1 = reinterpret_cast< uint16_t * >(argp);
}

%typemap(in) uint32_t {
   $1 = PyInt_AsLong($input);
}

/* Send "int *" to python as intp */
%pointer_functions(int, intp);
%{
    #include "zfm20.h"
    speed_t int_B57600 = B57600;
%}
%include "zfm20.h"
speed_t int_B57600 = B57600;
