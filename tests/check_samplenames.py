#!/usr/bin/python

import unittest as u
import re, fnmatch, os, sys

sampleMappingFile = '../doxy/samples.mapping.txt'
cSamplesDir = '../examples/c++/'
javaSamplesDir = '../examples/java/'


class SampleNames(u.TestCase):

    def test_existing_samples(self):
        missing_c_files = []
        missing_java_files = []

        with open (sampleMappingFile, "r") as f:
            for line in f:
                sampleNames = line.split();

                cSampleName = sampleNames[0]
                if not cSampleName.endswith('.cxx'):
                    continue

                javaSampleName = sampleNames[1]
                if not javaSampleName.endswith('.java'):
                    continue

                ok = False
                for file in os.listdir(cSamplesDir):
                    if file == cSampleName:
                        ok = True
                        break

                if not ok:
                    missing_c_files.append(cSampleName)

                ok = False
                for file in os.listdir(javaSamplesDir):
                    if file == javaSampleName:
                        ok = True
                        break

                if not ok:
                    missing_java_files.append(javaSampleName)

        self.assertEqual( len(missing_java_files) + len(missing_c_files), 0,
                "\nThe following files are missing from samples:\n" + \
                "\n".join(missing_c_files) + "\n" + "\n".join(missing_java_files))

if __name__ == '__main__':
          u.main()

