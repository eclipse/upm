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

import unittest
import os

class DuplicateHeadersTests(unittest.TestCase):
    '''Checks for duplicate header names. Since all sensor headers get
      installed as part of the API, duplicate headers can cause bad linking,
      namespace class collisions and so on.'''

    def test_duplicate_headers(self):
        # Keep a list of all the header files in library
        header_files = []

        # Recusively search cwd for headers and add them to the list
        for root, dirs, files in os.walk(os.curdir):
            dirs[:] = [d for d in dirs if d not in ['build']]
            for file in files:
                if file.endswith(('.h', '.hpp', '.hxx')):
                    header_files.append(file)

        # Test for duplicates
        duplicates = len(header_files) - len(set(header_files))
        self.assertEqual(duplicates, 4,
                "\nDuplicate headers: %d\n" % duplicates)

if __name__ == '__main__':
    unittest.main()
