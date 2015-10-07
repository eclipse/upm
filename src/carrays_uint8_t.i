// From the SWIG documentation:
//		Unlike #include, %include includes each file once
//		(and will not reload the file on subsequent %include declarations).
//		Therefore, it is not necessary to use include-guards in SWIG interfaces. 
// So you can include this file and .i files for other C types
%include "stdint.i"
%include "carrays.i"
%array_class(uint8_t, uint8Array);

// Adding these typemaps because SWIG is converting uint8, uint16, and uint32 into a short by default
// This forces SWIG to convert it correctly

#if (SWIG_JAVASCRIPT_V8)
%typemap(in) uint8_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint8Array, 0 |  0 );
  if (!SWIG_IsOK(res)) {
    SWIG_exception_fail(SWIG_ArgError(res), "failed to convert input to uint8");
  }
  $1 = (uint8_t *)(argp);
}
#endif

#if (SWIGPYTHON)
%typemap(in) uint8_t * {
  void *argp = 0 ;
  int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_uint8Array, 0 |  0 );
  if (!SWIG_IsOK(res)) {
    SWIG_exception_fail(SWIG_ArgError(res), "failed to convert input to uint8");
  }
  $1 = reinterpret_cast< uint8_t * >(argp);
}
#endif

//#elsif (SWIGJAVA)
