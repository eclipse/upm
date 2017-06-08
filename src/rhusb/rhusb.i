%include "iHumiditySensor.i"
%include "iTemperatureSensor.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"
JAVA_JNI_LOADLIBRARY(javaupm_rhusb)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
INHERIT_IUPMOBJECT(upm::RHUSB);
INHERIT_IHUMIDITYSENSOR(upm::RHUSB);
INHERIT_ITEMPERATURESENSOR(upm::RHUSB);
#endif
/* END Javascript syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "rhusb.hpp"
%}
%include "rhusb.hpp"
/* END Common SWIG syntax */
