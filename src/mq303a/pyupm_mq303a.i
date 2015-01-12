%module pyupm_mq303a
%include "../upm.i"

%feature("autodoc", "3");

%include "mq303a.h"
%{
    #include "mq303a.h"
%}
