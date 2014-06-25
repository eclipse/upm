%include "std_string.i"

%typemap(out) mraa_result_t = int;
