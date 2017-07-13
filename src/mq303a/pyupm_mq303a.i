// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mq303a
%include "../upm.i"

%include "mq303a.hpp"
%{
    #include "mq303a.hpp"
%}
