// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bmg160
%include "../upm.i"
%include "cpointer.i"
%include "../upm_vectortypes.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

#ifdef DOXYGEN
%include "bmg160_doc.i"
#endif

%include "bmg160_defs.h"
%include "bmg160.hpp"
%{
    #include "bmg160.hpp"
%}
