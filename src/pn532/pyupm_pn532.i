%module pyupm_pn532
%include "../upm.i"
%include "../carrays_uint8_t.i"

// Adding this typemap because SWIG is converting uint8, uint16, and uint32 into a short by default
// This forces SWIG to convert it correctly
%typemap(in) uint8_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp, SWIGTYPE_p_uint8Array, 0 |  0 );
  $1 = reinterpret_cast< uint8_t * >(argp);
}

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "pn532_doc.i"
#endif

%include "pn532.h"
%{
    #include "pn532.h"
%}
