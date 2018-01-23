%module javaupm_mpu9150
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "../java_buffer.i"
%include "std_vector.i"

%template(FloatVector) std::vector<float>;

%apply int {mraa::Edge};

%{
#include "mpu60x0.hpp"
#include "mpu9150.hpp"
%}
%include "mpu60x0.hpp"
%include "mpu9150.hpp"

%define GETTER get_gpioIRQ()
%enddef

JAVA_ADD_INSTALLISR_GPIO(upm::MPU60X0)

JAVA_JNI_LOADLIBRARY(javaupm_mpu9150)
