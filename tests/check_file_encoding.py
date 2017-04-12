#!/usr/bin/python

import unittest
import os
import chardet

target_exts = ['.h', '.hpp', '.hxx', '.txt']
valid_encodings = ['ascii', 'utf-8']

class EncodingTests(unittest.TestCase):
    '''Non-ascii/utf-8 encodings can cause failures on downstream tools
    such as documentation-generation and python2 module loading.  This
    class helps find those files which could cause an encoding problem'''

    def test_headers_ascii(self):
        '''Assert/print list of:
            file:linenumber offending line
        for all lines of matching files which are not in valid_encodings'''
        # Keep a map of files with alternate encodes to report
        invalid_files = {}

        # Recusively search cwd for files with target_exts
        for root, dirs, files in os.walk(os.curdir):
            # Work on full paths
            for file in files:
                file = os.path.join(root, file)

                # Skip any files not ending with target_exts
                if not any(file.lower().endswith(x) for x in target_exts):
                    continue

                # Check each with chardet
                with open(file, 'r') as f:
                    for ndx, line in enumerate(f):
                        result = chardet.detect(line)
                        if not result['encoding'] in valid_encodings:
                            if not invalid_files.has_key(file):
                                invalid_files[file] = []
                            invalid_files[file].append([ndx,line])

        # Sort the failures by filename
        skeys = list(invalid_files.keys())
        skeys.sort()
        invalid_lines = ''
        for fn in skeys:
            for line in invalid_files[fn]:
                invalid_lines += '%s:%d %s' % (fn, line[0], line[1])

        self.assertEqual( len(invalid_files), 0,
                "\nThe following modules have alternate encodings:\n" + \
                invalid_lines)

if __name__ == '__main__':
    unittest.main()
