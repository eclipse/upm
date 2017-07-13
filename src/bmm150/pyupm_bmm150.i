// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bmm150
%include "../upm.i"
%include "cpointer.i"
%include "../upm_vectortypes.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

#ifdef DOXYGEN
%include "bmm150_doc.i"
#endif

%include "bmm150_defs.h"
%include "bmm150.hpp"
%{
    #include "bmm150.hpp"
%}
