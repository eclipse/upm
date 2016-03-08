// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq135
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "mq135_doc.i"
#endif

%include "mq135.h"
%{
    #include "mq135.h"
%}
