// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_wt5001
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"

%rename("getVolumeNoParam")  getVolume();
%rename("getPlayStateNoParam") getPlayState();
%rename("getNumFilesOneParam")  getNumFiles(WT5001_PLAYSOURCE_T psrc);
%rename("getCurrentFileNoParam") getCurrentFile();

%feature("autodoc", "3");

%{
    #include "wt5001.hpp"
    speed_t int_B9600 = B9600;
%}
%include "wt5001.hpp"
speed_t int_B9600 = B9600;
