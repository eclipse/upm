#!/usr/bin/python
import unittest
import re, fnmatch, os, sys

# Example name map
sampleMappingFile = '../doxy/samples.mapping.txt'

# Example directories
sampledir = {}
sampledir['cxx'] = '../examples/c++/'
sampledir['java'] = '../examples/java/'
sampledir['js'] = '../examples/javascript/'
sampledir['py'] = '../examples/python/'

# Default to all targets
test_targets = ['java', 'js', 'py']

class SampleNames(unittest.TestCase):
    def test_existing_samples(self):
        # Dictionary for missing examples
        missing = {}

        # If test target/s were provided on the command line, use them
        if len(self.argv) > 0:
           test_targets = self.argv

        # Iterate over the mapping file and check for per-language examples
        with open (sampleMappingFile, "r") as f:
            for line in f:
                matched = {}
                fullpaths = {}
                # Split out the example names per target
                for target in test_targets:
                    match = re.match('(.*.cxx).*\t(\S+.%s)' % target, line)
                    if match:
                        matched['cxx'] = match.group(1)
                        matched[target] = match.group(2)

                # Need at least two target files to check
                if len(matched) < 2: continue

                # Fail if NO .cxx file to compare against
                self.assertTrue('cxx' in matched)

                # Update the full path to each sample file
                for k,v in matched.items():
                    fullpaths[k] = os.path.join(sampledir[k], v)

                # Use cxx base filename as testname
                basename = os.path.splitext(matched['cxx'])[0]

                missing[basename] = []

                # Check for all example filenames
                for target, filename in fullpaths.items():
                    if not os.path.exists(filename):
                        missing[basename] += [os.path.split(filename)[-1]]

        # Prune out tests with no missing files
        for k in missing.keys():
            if len(missing[k]) == 0: del missing[k]

        # Print sorted missing example filenames
        sk_missing = missing.keys()
        sk_missing.sort()
        self.assertEqual(len(missing), 0,
                '\n\nMissing examples:\n' +
                '\n'.join(['%s: %s' % (k, ', '.join(missing[k])) for k in sk_missing]))

if __name__ == '__main__':
    # Allow passing from argv
    SampleNames.argv = []
    for arg in sys.argv[1:]:
        SampleNames.argv.append(sys.argv.pop())
    unittest.main()
