#!/usr/bin/python

import unittest as u
import re, fnmatch, os

rootDir = '../build/src/'
swigtypeStr = 'SWIGTYPE'

class Clean(u.TestCase):

    def test_existing_swigtype(self):
        unclean = []

        for dirName in os.listdir(rootDir):
            dirPath = os.path.join(rootDir, dirName)
            if not os.path.isdir(dirPath):
                continue

            ok = True
            for subdir, dirs, files in os.walk(dirPath):
                if not ok:
                    break
                for fileName in files:
                    # Only look at JAVA wrappers
                    if not fileName.endswith('JAVA_wrap.cxx'): continue
                    if swigtypeStr in fileName:
                        unclean.append(dirName)
                        ok = False
                        break

        self.assertEqual( len(unclean), 0,
                "\nThe following modules have unclean Java bindings:\n" + \
                "\n".join(unclean) + "\n\n" + \
                "Consider adding them to the SWIGJAVA blacklist")

if __name__ == '__main__':
    u.main()
