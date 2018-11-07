%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i"
%include "../java_buffer.i"

%ignore installISR(int pin, void *, void *);

JAVA_JNI_LOADLIBRARY(javaupm_mcp2515)
JAVA_ADD_INSTALLISR_PIN(upm::MCP2515)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "../carrays_uint8_t.i"
%pointer_functions(float, floatp);

%{
#include "mcp2515_regs.h"
#include "mcp2515.hpp"
%}
%include "mcp2515_regs.h"
%include "mcp2515.hpp"
/* END Common SWIG syntax */
