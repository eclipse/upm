#ifdef SWIGPYTHON
%module (package="upm") pyupm_new_interfaces
#else
%module new_interfaces
#endif

#ifdef SWIGJAVA
    %include <swiginterface.i>
    %include "typemaps.i"
%{
#include <vector>
%}

    %import "../../src/upm_javastdvector.i"

    %typemap(javaimports) SWIGTYPE %{
import java.util.AbstractList;
import java.lang.Float;
    %}

    /* Using the JAVA class types outside the core package requires
        getCPtr to be public, modify that here */
    %typemap(javabody) SWIGTYPE %{
    private long swigCPtr;
    protected boolean swigCMemOwn;
    public $javaclassname(long cPtr, boolean cMemoryOwn) {
        swigCMemOwn = cMemoryOwn;
        swigCPtr = cPtr;
    }
    public static long getCPtr($javaclassname obj) {
        return (obj == null) ? 0 : obj.swigCPtr;
    }
    %}

    %typemap(javaout) std::vector<float> {
        return (AbstractList<Float>)(new $&javaclassname($jnicall, true));
    }
    %typemap(jstype) std::vector<float> "AbstractList<Float>"
    
    %template(floatVector) std::vector<float>;

    %interface_impl (upm::iClock);
    %interface_impl (upm::iCollision);
    %interface_impl (upm::iDistance);
    %interface_impl (upm::iDistanceInterrupter);
    %interface_impl (upm::iEC);
    %interface_impl (upm::iHallEffect);
    %interface_impl (upm::iHeartRate);
    %interface_impl (upm::iHumidity);
    %interface_impl (upm::iLight);
    %interface_impl (upm::iLineFinder);
    %interface_impl (upm::iMoisture);
    %interface_impl (upm::iMotion);
    %interface_impl (upm::iPressure);
    %interface_impl (upm::iTemperature);
    %interface_impl (upm::iAcceleration);
#endif

%{
    #include "iClock.hpp"
    #include "iCollision.hpp"
    #include "iDistance.hpp"
    #include "iDistanceInterrupter.hpp"
    #include "iEC.hpp"
    #include "iHallEffect.hpp"
    #include "iHeartRate.hpp"
    #include "iHumidity.hpp"
    #include "iLight.hpp"
    #include "iLineFinder.hpp"
    #include "iMoisture.hpp"
    #include "iMotion.hpp"
    #include "iPressure.hpp"
    #include "iTemperature.hpp"
    #include "iAcceleration.hpp"
%}

%include "iClock.hpp"
%include "iCollision.hpp"
%include "iDistance.hpp"
%include "iDistanceInterrupter.hpp"
%include "iEC.hpp"
%include "iHallEffect.hpp"
%include "iHeartRate.hpp"
%include "iHumidity.hpp"
%include "iLight.hpp"
%include "iLineFinder.hpp"
%include "iMoisture.hpp"
%include "iMotion.hpp"
%include "iPressure.hpp"
%include "iTemperature.hpp"
%include "iAcceleration.hpp"

/* Java-specific SWIG syntax */
#ifdef SWIGJAVA
%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_interfaces");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
#endif
