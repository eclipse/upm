// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mma7361
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%feature("autodoc", "3");

%include "mma7361.hpp"
%{
    #include "mma7361.hpp"
%}

