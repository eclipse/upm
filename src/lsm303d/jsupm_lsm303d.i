%module jsupm_lsm303d
%include "../upm.i"
%include "../upm_vectortypes.i"

%include "lsm303d_defs.h"
%include "lsm303d.hpp"
%{
    #include "lsm303d.hpp"
%}
