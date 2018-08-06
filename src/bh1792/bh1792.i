%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore InstallISR(mraa_gpio_edge_t, int, void *, void *);

%extend upm::BH1792 {
    void InstallISR(int pin, jobject runnable)
    {
        $self->InstallISR(pin, mraa_java_isr_callback, runnable);
    }
}

JAVA_JNI_LOADLIBRARY(javaupm_bh1792)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "std_vector.i"
%template(intVector) std::vector<int>;
%template(intVector2D) std::vector<std::vector<int>>;

%{
#include "bh1792.hpp"
%}
%include "bh1792.h"
%include "bh1792.hpp"
/* END Common SWIG syntax */
