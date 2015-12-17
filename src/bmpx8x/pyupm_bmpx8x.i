// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bmpx8x
%include "../upm.i"

%include "stdint.i"

%include "bmpx8x.h"
%{
    #include "bmpx8x.h"
%}

