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

%include "mma7660.h"
%{
    #include "mma7660.h"
%}
