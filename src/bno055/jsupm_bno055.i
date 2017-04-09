%module jsupm_bno055
%include "../upm.i"
%include "cpointer.i"
%include "../upm_vectortypes.i"

/* Send "int *" and "float *" to JS as intp and floatp, though
 * using the vector return (upm_vectortypes.i) functions instead of
 * the pointer argument functions is preferable.
 */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%include "bno055_regs.h"
%include "bno055.hpp"
%{
    #include "bno055.hpp"
%}
