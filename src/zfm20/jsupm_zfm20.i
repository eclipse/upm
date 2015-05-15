%module jsupm_zfm20
%include "../upm.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"
%include "cpointer.i"

%typemap(in) uint16_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint16Array, 0 |  0 );
  $1 = (uint16_t *)(argp);
}

/*$input is a v8::object, which inherits from v8::value */
%typemap(in) uint32_t {
   $1 = ($input)->Uint32Value();
}

/* Send "int *" to JavaScript as intp */
%pointer_functions(int, intp);
%{
    #include "zfm20.h"
    speed_t int_B57600 = B57600;
%}

%include "zfm20.h"
speed_t int_B57600 = B57600;
