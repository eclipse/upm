/* -----------------------------------------------------------------------------
 * java_exceptions.i
 *
 * SWIG library file providing java-specific exception handling in the upm library
 * ----------------------------------------------------------------------------- */

/*
 * Use this macro for functions that read data from a sensor and throw a 
 * std::runtime_error in case of a read failure
*/
%define READDATA_EXCEPTION(function)
%javaexception("java.io.IOException") function {
  try {
     $action
  } catch (std::runtime_error &e) {
    jclass clazz = jenv->FindClass("java/io/IOException");
    jenv->ThrowNew(clazz, e.what());
    return $null;
   }
}
%enddef
