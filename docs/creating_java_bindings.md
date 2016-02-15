Creating Java Bindings Guide
==============
* [Overview](#overview)
* [Tools of trade](#tools-of-trade)
* [Recommendations for the native API](#recommendations-for-the-native-api)
  * [Pointers](#pointers)
  * [Throwing Exceptions in Java](#throwing-exceptions-in-java)
* [Caveats & Challenges](#caveats--challenges)
  * [Wrapping C arrays with Java arrays](#wrapping-c-arrays-with-java-arrays)
  * [Wrapping unbound C arrays with Java arrays if array is output](#wrapping-unbound-c-arrays-with-java-arrays-if-array-is-output)
  * [Wrapping unbound C arrays with Java arrays if array is input](#wrapping-unbound-c-arrays-with-java-arrays-if-array-is-input)
  * [Implementing callbacks in Java](#implementing-callbacks-in-java)


##Overview

The "Creating Java Bindings Guide" serves as a basic tutorial for using the SWIG software development tool to create 'glue code' required for Java to call into C/C++ code. It contains: guides for dealing with type conversions, exception handling, callbacks; recommendations on how to write/modify the native API to avoid issues on the Java side, and also workarounds for those issues that can't be avoided.

This guide was created with the [upm](https://github.com/intel-iot-devkit/upm/) and [mraa](https://github.com/intel-iot-devkit/mraa) libraries in mind, and uses examples taken from these sources, but its usage can be extended to any project of creating Java bindings for C/C++ libraries.

##Tools of trade

[SWIG General Documentation](http://www.swig.org/Doc3.0/SWIGDocumentation.html)

[SWIG Java-specific Documentation](http://www.swig.org/Doc3.0/Java.html)


##Recommendations for the native API

###Pointers
As much as possible, avoid passing values/returning values through pointers given as as arguments to methods. As the Java language does not have pointers, SWIG provides a [workaround](http://www.swig.org/Doc3.0/Java.html#Java_tips_techniques) in the typemaps.i library.

####Alternatives:
1. Functions that read data from a driver, return it through a pointer given as argument, and return a bool value, should be __replaced by__ functions that return the value directly and throw a std::runtime_error if a read error occurs. E.g.:
  ```c++
  /*  
   * Function reads from sensor, places read value in variable bar and  
   * returns true if succesful. Function returns false if read failed.  
   */  
  bool func(int *bar); 
  ```
  __Replaced by:__
  ```c++
  /*  
   * Function reads from sensor and returns read value.  
   * Or throws std::runtime_error if a read error occurs  
   */  
  int func();  
  ```

2. Functions that return multiple values through pointers, that make sense to be grouped together into an array<sup>1</sup> (e.g. speed values, acceleration values), should be __replaced by__ functions that return a pointer to an array in which the elements are the returned values. Afterwards, [wrap the C array with a Java array](#wrapping-unbound-c-arrays-with-java-arrays-if-array-is-output). E.g.:
  ```c++
  /*  
  * Function returns the acceleration on the three  
  * axis in the given variables.  
  */  
  void getAccel(int *accelX, int *accelY, int *accelZ);  
  ```

  __Replaced by:__  
  ```c++
  /*  
   * Function returns the acceleration on the three  
   * axis as elements of a 3-element array.  
   */  
  int *getAccel();  
  ```

3. Functions that return N values through pointers, that do not make sense to grouped together (e.g. a general purpose function that returns both the light intensity and air pollution), should be __replaced by__ N functions (one for each value) that read only one specific value. E.g.:

  ```c++
  /*  
   * Function returns the light intensity and air pollution  
   */  
  void getData(int *light, int *air);  
  ```

  __Replaced by:__  
  ```c++
  int getLight();  
  int getAir();  
  ```

4. Functions that return N values through pointers; values that do not make sense to be grouped together, but are time dependent, and make sense to be read at the same time. For example, a sensor that reads air humidity and temperature. A user may want to know the temperature value _temp_ read at the exact moment the humidity value _humid_ was read. These should be __replaced by__ N+1 functions: a _getData()_ function that reads all values at the same time and stores them in global variables; and N getter functions, one for each value. E.g.

  ```c++
  /*  
   * Function returns the light intensity and air pollution  
   */  
  void getData(int *temp, int *humid);  
  ```

  __Replaced by:__  
  ```c++
  void getData();  
  int getTemp();  
  int getHumid();   
  ```

  <sup>1</sup>this depends on the interpretation of the returned data. For example, arguments that return the temperature and light intensity, don't make sense to be grouped into an array of size 2. But acceleration on the three axis can be grouped together in an array of size 3. where accelX is accel[0], accelY is accel[1], accelZ is accel[2].

__Notice:__
Sometimes, you may be required to write JNI code. Be aware of the difference between the C JNI calling syntax and the C++ JNI calling syntax.The C++ calling syntax will not compile as C and also vice versa. It is however possible to write JNI calls which will compile under both C and C++ and is covered in the [Typemaps for both C and C++ compilation](http://www.swig.org/Doc3.0/Java.html#Java_typemaps_for_c_and_cpp) section of the SWIG Documentation.


###Throwing Exceptions in Java
####Language independent:
The %exception directive allows you to define a general purpose exception handler. For example, you can specify the following:

```c++
%exception [method_name] {
    try {
        $action
    }
    catch (std::invalid_argument& e) {
        ... handle error ...
    }
}
```

If [method_name] is not specified then the directive is applied to all methods in its scope.

The usual thing you'd want to do is catch the C++ exception and throw an equivalent exception in your language.

The exception.i library file provides support for creating language independent exceptions in your interfaces. To use it, simply put an "%include exception.i" in your interface file. This provides a function SWIG_exception() that can be used to raise common language exceptions in a portable manner. For example :


```c++
// Language independent exception handler  
%include exception.i  

%exception {  
    try {  
            $action  
        } catch(OutOfMemory) {  
            SWIG_exception(SWIG_MemoryError, "Out of memory");  
    } catch(...) {  
            SWIG_exception(SWIG_RuntimeError,"Unknown exception");  
    }  
}  
```

In the upm library, the upm_exception.i interface file provides the functionality to catch common exceptions and propagate them through SWIG. It uses the exception.i library file and is language independent.

The upm_exception.i interface file is included in the upm.i file, therefor SWIG wraps all generated methods' body in a try-catch statement for the following exceptions:

* std::invalid_argument
* std::domain_error
* std::overflow_error
* std::out_of_range
* std::length_error
* std::logic_error
* std::bad_alloc
* std::runtime_error
* std::exception


####Java specific:
To throw a specific Java exception:

```c++
%exception {  
    try {  
        $action  
    } catch (std::out_of_range &e) {  
        jclass clazz = jenv->FindClass("java/lang/Exception");  
        jenv->ThrowNew(clazz, "Range error");  
        return $null;  
    }  
}  
```

Where FindClass and ThrowNew are [JNI functions](http://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html).

Java defines two tipes of exceptions: checked exception and unchecked exceptions (errors and runtime exceptions). Checked exceptions are subject to the [Catch or Specify Requirement](https://docs.oracle.com/javase/tutorial/essential/exceptions/catchOrDeclare.html).

The C++ compiler does not force the code to catch any exception.

The %exception directive does not specify if a method throws a checked exception (does not add classes to the throws clause). For this, the  %javaexception(classes) directive is used; where classes is a string containing one or more comma separated Java classes.

```c++
%javaexception("java.lang.Exception") {  
    try {  
        $action  
    } catch (std::out_of_range &e) {  
        jclass clazz = jenv->FindClass("java/lang/Exception");  
        jenv->ThrowNew(clazz, "Range error");  
        return $null;  
    }  
}  
```

In the upm library, the java_exceptions.i library file provides the functionality to catch exceptions and propagate them through SWIG as Java checked exceptions. The file provides SWIG wrappers, in the form of macros, that can be applied to methods.E.g. use the __READDATA_EXCEPTION(function)__ macro for functions that read data from a sensor and throw a std::runtime_error in case of a read failure. This will result in:

```java
void function throws IOException ();  
```

##Caveats & Challenges

###Wrapping C arrays with Java arrays
SWIG can wrap arrays in a more natural Java manner than the default by using the arrays_java.i library file. Just include this file into your SWIG interface file.

###Wrapping unbound C arrays with Java arrays if array is output
Functions that return arrays, return a pointer to that array. E.g.:

```c++
/*  
 * Function returns the acceleration on the three  
 * axis as elements of a 3-element array.  
 */  

int *getAccel();  
```

__SWIG:__  
```c++
%typemap(jni) int* "jintArray"  
%typemap(jstype) int* "int[]"  
%typemap(jtype) int* "int[]"  

%typemap(javaout) int* {  
    return $jnicall;  
}  

%typemap(out) int *getAccel {  
    $result = JCALL1(NewIntArray, jenv, 3);  
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const signed int*)$1);  
}  
```

###Wrapping unbound C arrays with Java arrays if array is input
In C, arrays are tipically passed as pointers, with an integer value representig the length of the array. In Java, the length of an array is always known, so the length argument is redundant. This example shows how to wrap the C array and also get rid the length argument. E.g.:

```c++
void func(uint8_t *buffer, int length);  
```

__SWIG:__  
```c++
%typemap(jtype) (uint8_t *buffer, int length) "byte[]"  
%typemap(jstype) (uint8_t *buffer, int length) "byte[]"  
%typemap(jni) (uint8_t *buffer, int length) "jbyteArray"  
%typemap(javain) (uint8_t *buffer, int length) "$javainput"  

%typemap(in,numinputs=1) (uint8_t *buffer, int length) {  
	$1 = JCALL2(GetByteArrayElements, jenv, $input, NULL);  
	$2 = JCALL1(GetArrayLength, jenv, $input);  
}  
```

!!!! There is a difference between TYPE *name and TYPE * name in typemaps!!!!!


###Implementing callbacks in Java
Callbacks in the UPM Java library (as well as the MRAA Java library) make use of the _void mraa\_java\_isr\_callback(void\* data\)_ method from MRAA.  

__Callbacks in the UPM Java library are implemented as follows (we use the a110x Hall Effect sensors as example):__

We extend the sensor class with another method, _installISR\(jobject runnable\)_, which is a wrapper over the original _installISR\(void \(\*isr\)\(void \*\), void \*arg\)_ method. This will install the _mraa\_java\_isr\_callback\(\)_ method as the interrupt service routine \(ISR\) to be called, with _jobject runnable_ as argument.

SWIGJAVA is a symbol that is always defined by SWIG when using Java. We enclose the _installISR\(jobject runnable\)_ method in a _\#if defined(SWIGJAVA)_ check, to ensure the code only exists when creating a wrapper for Java.

```c++
#if defined(SWIGJAVA)
void A110X::installISR(jobject runnable)
{
    installISR(mraa_java_isr_callback, runnable);
}
#endif
```

We hide the underlying method, _installISR\(void \(\*isr\)\(void \*\), void \*arg\)_ , and expose only the _installISR\(jobject runnable\)_ to SWIG, through the use of the SWIGJAVA symbol. When SWIGJAVA is defined, we change the access modifier of the underlying method to private.

```c++
public:
#if defined(SWIGJAVA)
    void installISR(jobject runnable);
#else
    void installISR(void (*isr)(void *), void *arg);
#endif
private:
#if defined(SWIGJAVA)
    void installISR(void (*isr)(void *), void *arg);
#endif
```

To use callback in java, we create a ISR class, which implements the Java Runnable interface, and we override the _run\(\)_ method with the code to be executed when the interrupt is received. An example for the a110x Hall sensor that increments a counter each time an interrupt is received:

```java
public class A110X_intrSample {
    public static int counter=0;
        
    public static void main(String[] args) throws InterruptedException {
        upm_a110x.A110X hall = new upm_a110x.A110X(2);

        A110XISR callback = new A110XISR();
        hall.installISR(callback);
        
        while(true){
            System.out.println("Counter: " + counter);
            Thread.sleep(1000);
        }
    }
}

class A110XISR implements Runnable {
    public A110XISR(){
        super();
    }
    public void run(){
        A110X_intrSample.counter++;
    }
}
```
####Issues with java callbacks and workarounds

__SWIGJAVA not defined at compile time__

Consider the following files:
* example.h - Header file for our source file
* example.cxx - Source file containing the class Example, for which we build java bindings
* example.i - The SWIG interface, that includes the example.h header file

The build process of a java module using SWIG is split into two steps:

1. Generating the intermediate files, from the SWIG interface file. This will produce the java class file (Example.java), the JNI file (exampleJNI.java) and wrapper file (example_wrap.cxx). The source file (example.cxx) is not needed in the first step.

  ```
swig -c++ -java example.i
  ```

2. Generating the shared library from the C++ sources and wrapper file
  ```
g++ -fPIC -c example.cxx example_wrap.cxx -I/usr/lib/jvm/java-1.8.0/include -I/usr/lib/jvm/java-1.8.0/include/linux
g++ -shared example_wrap.o sensor.o -o libexample.so
  ```


SWIGJAVA is always defined when SWIG parses the interface file, meaning it will be defined when it parses the header file (example.h) that is included in the interface file (example.i).
SWIG also adds the "#define SWIGJAVA" directive in the wrapper file (example_wrap.cxx).
However, in generating the shared library the SWIGJAVA symbol is only defined in the example_wrap.cxx file, because of the added "#define SWIGJAVA" directive. But we have also used the "#if defined(SWIGJAVA)" check in the source file (example.cxx), and thus need to define SWIGJAVA for it too. If we define the SWIGJAVA symbol as a compile flag, when compiling the source code to object code, the SWIGJAVA compile flag and #define SWIGJAVA" directive will clash and give a double definition warning (only a warning).

In this example it is simple to compile the two source codes separately, one with the compile flag, the other without, and then create the shared library (libexample.so).
But in a big automatic build like the java upm libraries, this may prove too hard or too complicated to do. A workaround to this would be to define a custom symbol (e.q. JAVACALLBACK in the upm library) and also test for it. In short, replace:
```c++
#if defined(SWIGJAVA)
```
by
```c++
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
```

