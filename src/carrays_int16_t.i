// From the SWIG documentation:
//		Unlike #include, %include includes each file once
//		(and will not reload the file on subsequent %include declarations).
//		Therefore, it is not necessary to use include-guards in SWIG interfaces. 
// So you can include this file and .i files for other C types
%include "stdint.i"
%include "carrays.i"
%array_class(int16_t, int16Array);
