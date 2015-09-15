%module pyupm_gas
%include "../upm.i"
%include "../carrays_uint16_t.i"

%typemap(in) uint16_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint16Array, 0 |  0 );
  $1 = reinterpret_cast< uint16_t * >(argp);
}

%feature("autodoc", "3");

%include "gas.h"
%{
    #include "gas.h"
%}

%include "mq2.h"
%{
    #include "mq2.h"
%}

%include "mq3.h"
%{
    #include "mq3.h"
%}

%include "mq4.h"
%{
    #include "mq4.h"
%}

%include "mq5.h"
%{
    #include "mq5.h"
%}

%include "mq6.h"
%{
    #include "mq6.h"
%}

%include "mq7.h"
%{
    #include "mq7.h"
%}

%include "mq8.h"
%{
    #include "mq8.h"
%}

%include "mq9.h"
%{
    #include "mq9.h"
%}

%include "tp401.h"
%{
    #include "tp401.h"
%}
