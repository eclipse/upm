DoxyPort - Porting Doxygen documentation from C/C++ code to SWIG-generated code
==============

[![Build Status](https://travis-ci.org/intel-iot-devkit/doxyport.svg?branch=master)](https://travis-ci.org/intel-iot-devkit/doxyport)

DoxyPort is a Python project which ports Doxygen documentation from C/C++ code
to SWIG-generated bidings; code. Currently, DoxyPort only supports Java
generated bindings. The project covers the need to reuse the documentation from
the C/C++ code in the generated SWIG bindings.

Setup
==============

The project comes with a [Makefile](../master/Makefile) which allows easy setup
on a target machine. The setup process depends on the existence of
``virtualenv`` which can be installed using ``pip install virtualenv``.

To setup the virtual environment, run ``make setup``.


Execution
==============

Before running [doxyport](../master/doxyport) on a source project, you need to
have run SWIG before in order to have the generated Java code already created.
DoxyPort creates a backup file called ``previous_name.java.orig`` next to each
Java file ``previous_name.java``.

[doxyport](../master/doxyport) uses the previously created Python environment
on execution. It requires one mandatory parameter, the name of a file
containing a list of SWIG interface files.


Parameters
==============

```
user@host:~/doc$ doxyport --help
usage: doxyport.py [-h] [-s SOURCE] [-d DESTINATION] [-o OUTPUT] file_list

positional arguments:
  file_list             List with SWIG interface files

optional arguments:
  -h, --help            show this help message and exit
  -s SOURCE, --source SOURCE
                        One or more paths where to look for C/C++ headers
  -d DESTINATION, --destination DESTINATION
                        One or more paths where to look for Java class
                        definitions
  -o OUTPUT, --output OUTPUT
                        Write a file with the list of parsed files
```
