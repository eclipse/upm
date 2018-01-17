%module javaupm_mcp2515
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "../java_buffer.i"
%include "std_string.i"

%{
    #include "mcp2515.hpp"
    #include "mcp2515_regs.h"
%}
%include "mcp2515_regs.h"
%include "mcp2515.hpp"

%ignore installISR(int pin, void *, void *);


%extend upm::MCP2515 {
    void installISR(int pin, jobject runnable)
    {
        $self->installISR(pin, mraa_java_isr_callback, runnable);
    }
}

JAVA_JNI_LOADLIBRARY(javaupm_mcp2515)
