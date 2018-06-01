%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
JAVA_JNI_LOADLIBRARY(javaupm_nmea_gps)
#endif
/* END Java syntax */

/* BEGIN Python syntax  ------------------------------------------------------- */
#ifdef SWIGPYTHON
/* Attach pythons __str__ method to a similar method in C++ */
%feature("python:slot", "tp_str", functype="reprfunc") upm::gps_fix::__str__;
%feature("python:slot", "tp_str", functype="reprfunc") upm::satellite::__str__;
%feature("python:slot", "tp_str", functype="reprfunc") upm::nmeatxt::__str__;
%feature("python:slot", "tp_str", functype="reprfunc") upm::NMEAGPS::__str__;
%feature("python:slot", "tp_str", functype="reprfunc") upm::coord_DD::__str__;
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "std_vector.i"

%{
#include "nmea_gps.hpp"
%}
%template(satellitevec) std::vector<upm::satellite>;
%include "nmea_gps.hpp"
/* END Common SWIG syntax */
