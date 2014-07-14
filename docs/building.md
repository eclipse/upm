Building UPM                         {#building}
============

UPM uses cmake in order to make compilation relatively painless. Cmake runs
build out of tree so the recommended way is to clone from git and make a build/
directory.

UPM will attempt to build all directories inside src/ and they must contain
individual CMakeLists.txt files.

~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake ..
make
~~~~~~~~~~~~~

Our cmake configure has a number of options, `cmake -i` will ask you all sorts
of interesting questions, you can disable swig modules, build documentation
etc...

Few recommended options:
Changing install path from /usr/local to /usr
-DCMAKE_INSTALL_PREFIX:PATH=/usr

Building debug build:
-DCMAKE_BUILD_TYPE=DEBUG

Using clang instead of gcc:
 -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang

Often developers are only interested in building one module or even just the
python/node module to do some quick testing using scripting. In order to do
this you need to use the target name for the python or node module you want to
rebuild. For example the lcd module target name is i2clcd. Therfore the python
module target name will be prefixed by _pyupm_. Just do the following to build
only that module. Modules not using the UPM cmake macros may have different
naming.

~~~~~~~~~~~~~
make _pyupm_i2clcd
~~~~~~~~~~~~~
