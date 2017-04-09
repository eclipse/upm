// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mma7660
%include "../upm.i"
%include "cpointer.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "mma7660_doc.i"
#endif

%include "mma7660_regs.h"
%include "mma7660.hpp"
%{
    #include "mma7660.hpp"
%}
