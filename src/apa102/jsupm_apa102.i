%module jsupm_apa102
 
%include "../upm.i"
%inline %{
    #include <node_buffer.h>
%}

%typemap(in) (uint8_t *colors) {
  if (!node::Buffer::HasInstance($input)) {
      SWIG_exception_fail(SWIG_ERROR, "Expected a node Buffer");
  }
  $1 = (uint8_t*) node::Buffer::Data($input);
}

%include "apa102.h" 

%{
    #include "apa102.h"
%}
