#!/usr/bin/env python
# Author: Mircea Bardac <mircea.bardac@intel.com>
# Copyright (c) 2015 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

"""Module DoxyPort"""

import sys
import os.path
import os
import shutil
import CppHeaderParser
import javalang
import argparse
import re

class CppClassContainer(object):
    """Class CppClassContainer"""
    def __init__(self, name):
        self.name = name
        self.methods = {}
        self.properties = {}
        self.namespace_doxygen = ""
        self.class_doxygen = ""

    def add_method(self, name, m_dict):
        if name not in self.methods:
            self.methods[name] = []
        self.methods[name].append(m_dict)

    def display(self):
        print self.methods

    def param_type_list(self, params):
        p_result = []
        for param in params:
            p_result.append(param["type"])
        return p_result

    def extract_class_doxygen(self, class_content):
        namespace_doxygen = []
        class_doxygen = []
        in_namespace = True
        if not "doxygen" in class_content:
            return (namespace_doxygen, class_doxygen)
        for line in class_content["doxygen"].split("\n"):
            if in_namespace and line.find("*//*") == 0:
                namespace_doxygen.append("*/")
                line = line[2:]
                in_namespace = False
            if in_namespace:
                namespace_doxygen.append(line)
            else:
                class_doxygen.append(line)
        if in_namespace:
            class_doxygen = namespace_doxygen
            namespace_doxygen = []
        return (namespace_doxygen, class_doxygen)

    def fill(self, class_content):
        # class_content
        # [
        #     'inherits',
        #     'line_number',
        #     'doxygen',
        #     'name',
        #     'parent',
        #     'abstract',
        #     'namespace',
        #     'declaration_method',
        #     'properties',
        #     'forward_declares',
        #     'typedefs',
        #     'structs',
        #     'enums',
        #     'final',
        #     'nested_classes',
        #     'methods'
        # ]
        self.namespace_doxygen, self.class_doxygen = self.extract_class_doxygen(class_content)
        #print class_content["methods"]["public"]
        #print "Public methods:"
        for public_method in class_content["methods"]["public"]:
            #print m["doxygen"]
            #print m["rtnType"], m["const"], m["static"], m["virtual"], m["name"]
            #print m["constructor"], m["destructor"], len(m["parameters"])
            #print param_type_list(m["parameters"])
            self.add_method(public_method["name"], {
                'return_type': public_method["rtnType"],
                'const': public_method["const"],
                'static': public_method["static"],
                'virtual': public_method["virtual"],
                'constructor': public_method["constructor"],
                'destructor': public_method["destructor"],
                'param_types': self.param_type_list(public_method["parameters"]),
                'doxygen': public_method.get("doxygen", None)
            })
        #print "Public properties:"
        for public_property in class_content["properties"]["public"]:
            #print m["doxygen"]
            #print m["rtnType"], m["const"], m["static"], m["name"]
            #print m
            self.properties[public_property["name"]] = {
                'type': public_property["type"],
                'static': public_property["static"],
                # TODO: check if there are doxygen comments for properties
                #'doxygen': public_property["doxygen"]
            }
        if len(class_content["methods"]["private"]) > 0:
            for private_method in class_content["methods"]["private"]:
                if "doxygen" in private_method and len(private_method["doxygen"]) > 0:
                    print "*** Private method \"%s\" in class \"%s\" has doxygen comment." %(private_method["name"], self.name)
        if len(class_content["properties"]["private"]) > 0:
            for private_property in class_content["properties"]["private"]:
                if "doxygen" in private_property and len(private_property["doxygen"]) > 0:
                    print "*** Private property \"%s\" in class \"%s\" has doxygen comment." %(private_property["name"], self.name)
        if len(class_content["typedefs"]["public"]) > 0:
            for public_typedef in class_content["typedefs"]["public"]:
                if "doxygen" in public_typedef and len(public_typedef["doxygen"]) > 0:
                    print "*** Public typedef \"%s\" in class \"%s\" has doxygen comment." %(public_typedef["name"], self.name)
        if len(class_content["structs"]["public"]) > 0:
            for public_struct in class_content["structs"]["public"]:
                if "doxygen" in public_struct and len(public_struct["doxygen"]) > 0:
                    print "*** Public struct \"%s\" in class \"%s\" has doxygen comment." %(public_struct["name"], self.name)
        if len(class_content["enums"]["public"]) > 0:
            for public_enum in class_content["enums"]["public"]:
                if "doxygen" in public_enum and len(public_enum["doxygen"]) > 0:
                    print "*** Public enum \"%s\" in class \"%s\" has doxygen comment." %(public_enum["name"], self.name)
        if len(class_content["nested_classes"]) > 0:
            print "*** Class \"%s\" has %d public nested classes." %(self.name, len(class_content["nested_classes"]))
            #for c in class_content["nested_classes"]:
            #    print "    * %s" %(c["name"])
            #    for x in c.keys():
            #        print "      * %s: %s" %(x, c[x])

    def get_method_doxygen(self, method_name, method_declaration):
        if not method_name in self.methods: return ""
        method_list = self.methods[method_name]
        #print
        #print "!! Looking for:", method_declaration
        for cpp_m_declaration in method_list:
            #print ">>", method_name, "=", cpp_m_declaration
            if len(method_declaration["param_types"]) == len(cpp_m_declaration["param_types"]):
                if method_declaration["constructor"] != cpp_m_declaration["constructor"]: continue
                if method_declaration["destructor"] != cpp_m_declaration["destructor"]: continue
                #print "Found doxygen method:", method_name, len(cpp_m_declaration["param_types"])
                return cpp_m_declaration["doxygen"]
        return None # Not found

class JavaClassContainer(object):
    """Class JavaClassContainer"""
    def add_field(self, name, declaration):
        self.fields[name] = declaration
        #print n, d

    def add_method(self, method, declaration):
        if method not in self.methods:
            self.methods[method] = []
        self.methods[method].append(declaration)
        #print "method:", m, d

    def get_type(self, type_param):
        if type_param == None: return "void"
        if type_param.name == "boolean": return "bool"
        return type_param.name

    def param_type_list(self, params):
        p_result = []
        for param in params:
            p_result.append(self.get_type(param.type))
        return p_result

    def add_field_declaration(self, declaration):
        dec = {
            'type': declaration.type.name,
            'line_position': declaration._position[0],
            'private': 'private' in declaration.modifiers,
            'protected': 'protected' in declaration.modifiers,
            'public': 'public' in declaration.modifiers,
            'static': 'static' in declaration.modifiers,
            'const': 'const' in declaration.modifiers,
        }
        for field in declaration.declarators:
            self.add_field(field.name, dec)

    def add_constructor_declaration(self, declaration):
        dec = {
            'line_position': declaration._position[0],
            'private': 'private' in declaration.modifiers,
            'protected': 'protected' in declaration.modifiers,
            'public': 'public' in declaration.modifiers,
            'static': 'static' in declaration.modifiers,
            'const': 'const' in declaration.modifiers,
            'constructor': True,
            'destructor': False,
            'return_type': "void",
            'param_types': self.param_type_list(declaration.parameters)
        }
        self.add_method(declaration.name, dec)

    def add_method_declaration(self, declaration):
        dec = {
            'line_position': declaration._position[0],
            'private': 'private' in declaration.modifiers,
            'protected': 'protected' in declaration.modifiers,
            'public': 'public' in declaration.modifiers,
            'static': 'static' in declaration.modifiers,
            'const': 'const' in declaration.modifiers,
            'constructor': False,
            'destructor': False,
            'return_type': self.get_type(declaration.return_type),
            'param_types': self.param_type_list(declaration.parameters)
        }
        self.add_method(declaration.name, dec)

    def doxygen_post_process(self, doxygen_comment, params={}):
        if params.get("snippet_file_mapping", None) == None:
            # No post-processing parameters passed
            return doxygen_comment
        def file_lookup(file_name, dict_name):
            """file_lookup"""
            ref = dict_name.get(file_name, None)
            if ref is None:
                print "* Warning: No match found for referenced snippet in '%s'" %(file_name)
                return file_name
            return ref
        def file_replace(match_object):
            """file_replace"""
            file_name = match_object.group(1)
            new_name = file_lookup(file_name, params.get("snippet_file_mapping", {}))
            return "@snippet %s " %(new_name)
        doxygen_comment_out = []
        # TODO: must ensure all comments are stored in the same way
        # (not as lists or as strings at the same time)
        # Ugly temporary fix below
        if type(doxygen_comment) == str: doxygen_comment = [doxygen_comment]
        for line in doxygen_comment:
            # code for displaying regexp matches
            #if line.find("@snippet")>=0: print "!!!!!", line
            #if self.snippet_pattern.search(line): print "?????", line
            r = re.sub(self.snippet_pattern, file_replace, line)
            #if line.find("@snippet")>=0: print "!!!!>", r
            doxygen_comment_out.append(r)
        return doxygen_comment_out

    def attach_doxygen(self, cpp_cc, params):
        # if cpp_cc.namespace_doxygen != "":
        #     # Merge namespace doxygen and class doxygen comments
        #     t = cpp_cc.class_doxygen
        #     cpp_cc.class_doxygen = []
        #     cpp_cc.class_doxygen.extend(cpp_cc.namespace_doxygen[:-1])
        #     if len(t) > 0:
        #         cpp_cc.class_doxygen.extend(t[1:])
        #     cpp_cc.namespace_doxygen = ""
        if self.package_line != -1 and cpp_cc.namespace_doxygen != "":
            self.doxygen_map[self.package_line] = self.doxygen_post_process(cpp_cc.namespace_doxygen, params)
        if self.class_line != -1 and cpp_cc.class_doxygen != "":
            self.doxygen_map[self.class_line] = self.doxygen_post_process(cpp_cc.class_doxygen, params)
        for method in self.methods.keys():
            method_list = self.methods[method]
            for m_declaration in method_list:
                d = cpp_cc.get_method_doxygen(method, m_declaration)
                if d == None: # Not found
                    if m_declaration["protected"]:
                        print "* Doxygen not found for method %s (possible SWIG-generated internal use code)" %(method)
                    else:
                        print "* Doxygen not found for method %s(%d params) @ line %d" %(method, len(m_declaration["param_types"]), m_declaration["line_position"])
                    d = ""
                if d != "":
                    print "* Attaching doxygen to method %s(%d params) @ line %d" %(method, len(m_declaration["param_types"]), m_declaration["line_position"])
                    self.doxygen_map[m_declaration["line_position"]] = self.doxygen_post_process(d, params)
        # TODO: Attach Doxygen to fields as well

    def convert_protected_to_private(self):
        for method in self.methods.keys():
            method_list = self.methods[method]
            for m_declaration in method_list:
                if m_declaration["protected"]:
                    self.convert_protected_to_private_list.append(m_declaration["line_position"])
        for field_name, field_declaration in self.fields.items():
            if field_declaration.get("protected", False):
                self.convert_protected_to_private_list.append(field_declaration["line_position"])
        #print "!!!", self.class_declaration, self.convert_protected_to_private_list

    def convert_protected_to_private_text(self, text):
        r = re.sub(self.protected_pattern, " private ", text)
        return r

    def insert_doxygen(self, out_file, doxygen_line):
        """
        insert at the current position in out_file the doxygen comment recorded
        for line doxygen_line
        """
        d = self.doxygen_map[doxygen_line]
        if type(d) == str:
            out_file.write("%s\n" %(d))
            return
        for new_line in d:
            out_file.write("%s\n" %(new_line))

    def rewrite_class_file(self, params):
        java_file = "%s" %(self.filename)
        orig_java_file = "%s.orig" %(self.filename)
        shutil.copyfile(java_file, orig_java_file)
        print "Rewriting %s (%d doxygen attachments)." %(java_file, len(self.doxygen_map))
        #print self.doxygen_map
        i = 1
        insert_lines = self.doxygen_map.keys()
        out_file = open(java_file, "wt")

        # Add note to header to identify .java files touched by doxyport.
        # If ANY documentation lines exist for this file, add a small
        # snippet at the beginning to show it was updated.
        if len(insert_lines) > 0:
            out_file.write('/* CXX header documentation injected w/%s */\n\n' %
                os.path.basename(sys.argv[0]))

        for l in open(orig_java_file).readlines():
            # Check if Doxygen comments need to be appended before this line
            if i in insert_lines:
                self.insert_doxygen(out_file, i)
            # Check if 'protected' needs to be changed to 'private' for this line
            if params["convert-protected-to-private"] and i in self.convert_protected_to_private_list:
                l = self.convert_protected_to_private_text(l)
            out_file.write("%s" %(l))
            i += 1
        if params["convert-protected-to-private"]:
            print "> Converted %d methods and fields from 'private' to 'protected'." %(len(self.convert_protected_to_private_list))
        print
        out_file.close()

    def __init__(self, filename, cd):
        self.filename = filename
        self.class_declaration = cd
        self.name = cd.name
        self.fields = {}
        self.methods = {}
        self.doxygen_map = {}
        self.convert_protected_to_private_list = []
        self.package_line = -1
        self.class_line = -1
        self.snippet_pattern = re.compile("@snippet\s([a-zA-Z0-9_\.\-]+)\s")
        self.protected_pattern = re.compile("\sprotected\s")
        c = 0
        with open(filename) as f:
            for l in f.readlines():
                c += 1
                if l.startswith("package "): self.package_line = c
                if l.startswith("public class "): self.class_line = c
        print "Class name: %s" %(self.name)
        for declaration in cd.body:
            if type(declaration) == javalang.tree.FieldDeclaration:
                self.add_field_declaration(declaration)
            if type(declaration) == javalang.tree.ConstructorDeclaration:
                self.add_constructor_declaration(declaration)
            if type(declaration) == javalang.tree.MethodDeclaration:
                self.add_method_declaration(declaration)

class SwigProcessor(object):
    """Class SwigProcessor"""
    def __init__(self, source_loc, destination_loc, params):
        self.cpp_classes = {}
        self.java_classes = {}
        self.source_loc = source_loc
        self.destination_loc = destination_loc
        self.parsed_destination_files = []
        self.params = params

    def find_source(self, file_name):
        if os.path.isfile(file_name):
            return file_name
        base_file_name = os.path.basename(file_name)
        for p in self.source_loc:
            path = "%s/%s" %(p, base_file_name)
            if os.path.isfile(path):
                return path
        return None

    def find_destination(self, root_path, class_name):
        relative_path = "%s/%s.java" %(root_path, class_name)
        search_paths = []
        #path = "build/%s/%s.java" %(root_path, class_name)
        #search_paths.append(path)
        #if os.path.isfile(path):
        #    return (path, search_paths)
        base_file_name = "%s.java" %(class_name)
        for p in self.destination_loc:
            # Try to see if we can just prepend the given destinations
            # and consider we are being given a relative path
            path = "%s/%s" %(p, relative_path)
            search_paths.append(path)
            if os.path.isfile(path):
                return (path, search_paths)
            # Search directly the base file name in the specified destinations
            path = "%s/%s" %(p, base_file_name)
            search_paths.append(path)
            if os.path.isfile(path):
                return (path, search_paths)
        return (None, search_paths)

    def process_java(self, class_file):
        ret = {}
        print "Parsing class file: %s" %(class_file)
        tree = javalang.parse.parse(open(class_file).read())
        package_name = ""
        if tree.package is not None:
            package_name = tree.package.name
        print "Package name: %s" %(package_name)
        count = 0
        for c in tree.types:
            if type(c) == javalang.tree.ClassDeclaration:
                jc = JavaClassContainer(class_file, c)
                ret.update({jc.name: jc})
                count += 1
        if count > 1:
            print "*** Warning: Found %d Java class definitions in \"%s\"." %(count, class_file)
        return ret

    def process_header(self, root_path, header_file):
        ret = {}
        header_loc = "%s/%s" %(root_path, header_file)
        header_loc = self.find_source(header_loc)
        if header_loc == None:
            print "Unable to locate: %s\nSearch paths:\n\t%s" % \
                    (header_file, "\n\t".join([root_path] + self.source_loc))
            return
        print "Processing header: %s" %(header_loc)
        try:
            parser = CppHeaderParser.CppHeader(header_loc)
        except CppHeaderParser.CppParseError as e:
            print "* Error parsing: %s" %(header_loc)
            print(e)
            return ret
            #sys.exit(3)
        except UnboundLocalError as e:
            # The CppHeaderParser crashes on some syntax, this catches the error.
            # Issue not fixed upstream, using this as a workaround.
            print "* Error parsing: %s" %(header_loc)
            print(e)
            return ret

        for class_name, class_content in parser.classes.iteritems():
            c = CppClassContainer(class_name)
            c.fill(class_content)
            #c.display()
            ret[class_name] = c
        return ret

    def process_swig(self, file_name):
        ignore_line = False
        root_path = os.path.dirname(file_name)
        for line in open(file_name).readlines():
            line = line.strip()
            if line.startswith("//"): continue
            if line.startswith("/*"): ignore_line = True
            if line.endswith("*/"):
                ignore_line = False
                continue
            if ignore_line: continue
            if line.startswith("%include"): # This is an include line
                include_file = line.split()[1].strip("\"")
                if include_file.endswith(".i"):
                    print "Ignoring recursive SWIG interface inclusions (%s)" %(include_file)
                    continue
                if include_file.lower().endswith(".h"):
                    try:
                        self.cpp_classes.update(self.process_header(root_path, include_file))
                    except:
                        print 'Failed at path: %s, file: %s' % (root_path, include_file)
                        raise
                if include_file.lower().endswith(".hpp"):
                    try:
                        self.cpp_classes.update(self.process_header(root_path, include_file))
                    except:
                        print 'Failed at path: %s, file: %s' % (root_path, include_file)
                        raise
        for class_name in self.cpp_classes.keys():
            (java_loc, search_paths) = self.find_destination(root_path, class_name)
            if java_loc == None:
                print "* Warning: Unable to find Java class definition file for class \"%s\"" %(class_name)
                print "*** Searched locations:", search_paths
                continue
            java_file = java_loc
            orig_java_file = "%s.orig" %(java_file)
            # Restore class file from .orig file (we're rebuilding, trust backups)
            if os.path.isfile(orig_java_file):
                shutil.copyfile(orig_java_file, java_file)
            self.java_classes.update(self.process_java(java_file))
            self.parsed_destination_files.append(java_file)
        #print self.cpp_classes
        #print self.java_classes

    def push_doxygen(self):
        for class_name in self.cpp_classes.keys():
            if class_name in self.java_classes:
                #print "common class", class_name
                self.java_classes[class_name].attach_doxygen(self.cpp_classes[class_name], self.params)
                if self.params["convert-protected-to-private"]:
                    self.java_classes[class_name].convert_protected_to_private()
                self.java_classes[class_name].rewrite_class_file(self.params)

    def append_destination_files(self, output_file_handler):
        if output_file_handler == None: return
        for l in self.parsed_destination_files:
            output_file_handler.write("%s\n" %(l))

def includes_from_cmake(swig_file, cmake_json):
    ''' Parse target CMake json compile commands file.  Return a list of header
    include directories where the build directory name matches the directory
    name for the provided swig_file'''
    import re
    match = re.match(r'.*/src/(.*)/.*\.i', swig_file)
    dirs = []
    if match:
        # For each directory in the JSON which matches, grab include dirs
        for cmd in (item['command'] for item in cmake_json if
                (item['directory'].endswith(match.group(1)) and
                    item['file'].endswith('JAVA_wrap.cxx'))):
            includes = re.findall('-I(\S+)', cmd)
            if includes:
                dirs += includes
    # Get the sensor library directory name
    return set(dirs)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("file_list", help="List with SWIG interface files")
    parser.add_argument("-s", "--source", default="", help="One or more paths where to look for C/C++ headers")
    parser.add_argument("-c", "--cmake", help="CMake json compile commands file")
    parser.add_argument("-d", "--destination", default=None, help="One or more paths where to look for Java class definitions")
    parser.add_argument("-o", "--output", help="Write a file with the list of parsed files")
    parser.add_argument("-m", "--mapping", default=None, help="File mapping for @snippet references")
    parser.add_argument("--convert-protected-to-private", action='store_true', help="Convert protected fields & methods to private (this can break builds if resulting code is compiled)")
    args = parser.parse_args()

    swig_list_file = args.file_list
    if not os.path.isfile(swig_list_file):
        print "File not found: %s" %(swig_list_file)
        sys.exit(2)

    output_file_handler = None
    if args.output != None:
        try:
            output_file_handler = open(args.output, "wt")
        except:
            pass

    params = {
        "convert-protected-to-private": args.convert_protected_to_private,
        "snippet_file_mapping": None
    }

    if args.mapping is not None:
        mapping = {}
        with open(args.mapping) as f:
            for line in f:
                if len(line) == 0: continue
                if line[0] == "#": continue
                # Split mapping file on any whitespace (vs. # only tabs)
                # since users often put spaces here.
                words = line.strip().split()
                mapping[words[0]] = words[1]
        params["snippet_file_mapping"] = mapping

    dest = []
    if args.destination is not None:
        dest = args.destination.split(",")

    # Use a cmake json compile file for additional hints/includes
    if args.cmake:
        import json
        args.cmake = json.load(open(args.cmake, 'r'))

    for ndx, swig_file in enumerate(open(swig_list_file).readlines()):
        swig_file = swig_file.strip()
        # If args.cmake provided, add to args.source for this directory
        search_dirs = args.source.split(',')
        if args.cmake:
            search_dirs += includes_from_cmake(swig_file, args.cmake)

        sp = SwigProcessor(search_dirs, dest, params)
        sp.process_swig(swig_file)
        sp.push_doxygen()
        sp.append_destination_files(output_file_handler)

    if output_file_handler != None:
        output_file_handler.close()

    print 'Finished processing %d SWIG interface files...\n' % ndx
