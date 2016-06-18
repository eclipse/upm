%module jsupm_bmx055
%include "../upm.i"
%include "cpointer.i"

/* Send "int *" and "float *" to JavaScript as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "bma250e.hpp"
%{
    #include "bma250e.hpp"
%}

%include "bmg160.hpp"
%{
    #include "bmg160.hpp"
%}

%include "bmm150.hpp"
%{
    #include "bmm150.hpp"
%}

%include "bmx055.hpp"
%{
    #include "bmx055.hpp"
%}

%include "bmc150.hpp"
%{
    #include "bmc150.hpp"
%}

%include "bmi055.hpp"
%{
    #include "bmi055.hpp"
%}
