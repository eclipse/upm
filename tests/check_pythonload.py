#!/usr/bin/python
import unittest
import imp
import glob
import os, sys

# Skip individual modules based on module name.  For example,
# pyupm_ozw will skip pyupm_ozw.py.
blacklist = [
             # OpenZwave-dependent modules
             'pyupm_ozw', 'aeotecdw2e', 'tzemt400', 'aeotecdsb09104',
             'aeotecss6', 'aeotecsdg2', 'ozwdump',
             # Requires mraa python module
             'curieimu',
             # Requires modbus
             'h803x', 'hwxpxx', 't3311',
             # Requires bacnet
             'e50hx', 'tb7300', 't8100',
             # Requires PIL
             'make_oled_pic'
             ]

class loadModule(unittest.TestCase):
    ''' The loadModule class loads all module which match a search string
    provided via argv.  If any of the target modules fails loading, this
    class will assert and provide a list of failing modules.'''
    def test_load_module(self):
        # Python version provided on the command line
        py_search_str = '../build/src/*/python2.7/*.py'
        if len(self.argv) > 0:
           py_search_str = self.argv[0]

        # Make sure the search string has a full path
        full_py_search_str = os.path.realpath(py_search_str)

        # Get all python modules matching full_py_search_str
        pyfiles = glob.glob(full_py_search_str)

        # Fail if no modules to test
        self.assertNotEqual(len(pyfiles), 0,
                'Failed to find any %s modules in %s' % \
                (py_search_str, full_py_search_str));

        # Test load each module
        failures = {}
        for pyfile in pyfiles:
            module = os.path.splitext(os.path.basename(pyfile))[0]

            # Don't load blacklisted modules
            if module in blacklist:
                print('Skipping blacklisted %s ...' % pyfile)
                continue

            try:
                pyfile = imp.load_source(module, pyfile)
            except Exception as x:
                failures[pyfile] = x

        skeys = list(failures.keys())
        skeys.sort()
        self.assertEqual(len(failures), 0,
                '\n\n%s' % '\n'.join((['%s=%s' % (k, os.environ[k]) for k in list(os.environ.keys())])) +
                '\npython %s\n' % ' '.join(sys.version.strip().split()) +
                '\nFailed to load %d modules:\n' % len(failures) +
                '\n'.join(['%s: %s' % (k, failures[k]) for k in skeys]))

if __name__ == '__main__':
    # Allow passing from argv
    loadModule.argv = []
    for arg in sys.argv[1:]:
        loadModule.argv.append(sys.argv.pop())
    unittest.main()
