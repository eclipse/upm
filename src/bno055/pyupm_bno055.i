// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bno055
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "bno055_doc.i"
#endif

%include "bno055.hpp"
%{
    #include "bno055.hpp"
%}
