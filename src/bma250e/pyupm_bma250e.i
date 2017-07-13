// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bma250e
%include "../upm.i"
%include "cpointer.i"
%include "../upm_vectortypes.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

#ifdef DOXYGEN
%include "bma250e_doc.i"
#endif

%include "bma250e_defs.h"
%include "bma250e.hpp"
%{
    #include "bma250e.hpp"
%}
