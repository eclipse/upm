// Include doxygen-generated documentation
%module(directors="1", threads="1") pyupm_max30100
%include "pyupm_doxy2swig.i"
%include "../upm.i"

%{
    #include "max30100.hpp"
%}

%feature("director") upm::Callback;
%include "max30100_regs.h"
%include "max30100.hpp"
