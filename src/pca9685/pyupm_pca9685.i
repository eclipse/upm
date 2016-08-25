// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_pca9685
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "pca9685_doc.i"
#endif

%include "pca9685.hpp"
%{
    #include "pca9685.hpp"
%}
