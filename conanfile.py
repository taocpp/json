#!/usr/bin/env python
# -*- coding: utf-8 -*-
from conans import ConanFile, CMake

class JSONConan(ConanFile):
    name = "json"
    description = "C++17 header-only JSON library "
    homepage = "https://github.com/taocpp/json"
    url = homepage
    license = "MIT"
    author = "taocpp@icemx.net"
    exports = "LICENSE"
    exports_sources = "include/*", "CMakeLists.txt"
    no_copy_source = True

    def build(self):
        pass

    def package(self):
        cmake = CMake(self)
        cmake.definitions["TAOCPP_JSON_BUILD_TESTS"] = False
        cmake.definitions["TAOCPP_JSON_BUILD_EXAMPLES"] = False
        cmake.definitions["TAOCPP_JSON_INSTALL_DOC_DIR"] = "licenses"
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
