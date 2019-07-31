#!/usr/bin/python
#
# Copyright (c) 2017-2019 Intel Corporation
#
# This program and the accompanying materials are made available under the
# terms of the The MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT
#
from __future__ import print_function

import unittest as u
import re, fnmatch, os

rootDir = '../src/'
prefix = """
%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary(\""""

suffix = """\");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \\n" + e);
            System.exit(1);
        }
    }
%}"""

class AutoLoadLibrary(u.TestCase):

    def test_existing_snippet(self):
        broken_modules = []

        for subdir, dirs, files in os.walk(rootDir):
            for fileName in files:
                if fnmatch.fnmatch(fileName, '*.i'):
                    moduleName = fileName[:-2]
                    snippet = prefix + moduleName + suffix

                    with open(os.path.join(subdir, fileName), "rU") as f:
                        #read in entire file
                        data = f.read()
                        #Make sure it is module
                        if not '%module' in data:
                            print ("%%module not found in %s, skipping" % fileName)
                            continue
                        # Check for the autoload code or the autoload macro
                        if (not 'JAVA_JNI_LOADLIBRARY(javaupm_' in data) and \
                                (not snippet in data):
                            broken_modules.append('%s: %s' % \
                                    (moduleName, os.path.join(subdir, fileName)))

        self.assertEqual( len(broken_modules), 0, \
                "\nThe following modules do not contain the standard auto load library code:\n" + \
                "\n".join(broken_modules) + \
                "\nConsider adding the following snippet to the SWIG interface file:\n" + \
                prefix + "<module_name>" + suffix)


if __name__ == '__main__':
      u.main()
