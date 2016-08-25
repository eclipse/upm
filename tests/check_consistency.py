#!/usr/bin/python

import unittest as u
import re, fnmatch, os

rootDir = '../src/'
javaBlacklistFile = '../src/javaswig_blacklist'
pythonBlacklistFile = '../src/pythonswig_blacklist'
nodeBlacklistFile = '../src/nodeswig_blacklist'


class BlacklistConsistency(u.TestCase):

    def test_java_blacklist(self):

        with open(javaBlacklistFile) as f:
            blacklist = [line.rstrip('\n') for line in f]

            for libraryName in blacklist:
                files = os.listdir( os.path.join(rootDir, libraryName))
                interfaceFileName = "javaupm_" + libraryName + ".i"

                if interfaceFileName in files:
                    self.fail("\n" + libraryName + " is in javaswig blacklist.\n" + \
                            "Remove it from blacklist or remove " + \
                            interfaceFileName + " from sources.")


    def test_python_blacklist(self):

        with open(pythonBlacklistFile) as f:
            blacklist = [line.rstrip('\n') for line in f]

            for libraryName in blacklist:
                files = os.listdir( os.path.join(rootDir, libraryName))
                interfaceFileName = "pyupm_" + libraryName + ".i"

                if interfaceFileName in files:
                    self.fail("\n" + libraryName + " is in pythonswig blacklist.\n" + \
                            "Remove it from blacklist or remove " + \
                            interfaceFileName + " from sources.")


    def test_node_blacklist(self):

        with open(nodeBlacklistFile) as f:
            blacklist = [line.rstrip('\n') for line in f]

            for libraryName in blacklist:
                files = os.listdir( os.path.join(rootDir, libraryName))
                interfaceFileName = "jsupm_" + libraryName + ".i"

                if interfaceFileName in files:
                    self.fail("\n" + libraryName + " is in nodeswig blacklist.\n" + \
                            "Remove it from blacklist or remove " + \
                            interfaceFileName + " from sources.")


if __name__ == '__main__':
    u.main()
