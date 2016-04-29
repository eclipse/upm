// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bmp280
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "bmp280_doc.i"
#endif

%include "bmp280.hpp"
%{
    #include "bmp280.hpp"
%}
