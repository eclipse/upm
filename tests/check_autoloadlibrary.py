#!/usr/bin/python

import unittest as u
import re, fnmatch, os

rootDir = '../src/'
prefix = """
%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary(\""""

sufix = """\");
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
                if fnmatch.fnmatch(fileName, 'javaupm_*.i'):
                    moduleName = fileName[:-2]
                    snippet = prefix + moduleName + sufix

                    with open(os.path.join(subdir, fileName), "r") as f:
                        if  not snippet in f.read():
                            broken_modules.append('%s: %s' % \
                                    (moduleName, os.path.join(subdir, fileName)))

        self.assertEqual( len(broken_modules), 0,
                "\nThe following modules do not contain the standard auto load library code:\n" + \
                "\n".join(broken_modules) + \
                "\nConsider adding the following snippet to the SWIG interface file:\n" + \
                prefix + "<module_name>" + sufix)


if __name__ == '__main__':
      u.main()
