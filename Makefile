# Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
# Please see LICENSE for license or visit https://github.com/taocpp/json

ifeq ($(OS),Windows_NT)
UNAME_S := $(OS)
else
UNAME_S := $(shell uname -s)
endif

# For Darwin (Mac OS X) we assume that the default compiler
# clang++ is used; when $(CXX) is some version of g++, then
# $(TAOCPPJSON_CXXSTD) has to be set to -std=c++11 (or newer) so
# that -stdlib=libc++ is not automatically added.

ifeq ($(TAOCPPJSON_CXXSTD),)
TAOCPPJSON_CXXSTD := -std=c++11
ifeq ($(UNAME_S),Darwin)
TAOCPPJSON_CXXSTD += -stdlib=libc++
endif
endif

# Ensure strict standard compliance and no warnings, can be
# changed if desired.

TAOCPPJSON_CPPFLAGS ?= -pedantic
TAOCPPJSON_CXXFLAGS ?= -Wall -Wextra -Werror -O3

.PHONY: all clean

SOURCES := $(wildcard */*/*/*.cc)
DEPENDS := $(SOURCES:%.cc=build/%.d)
BINARIES := $(SOURCES:%.cc=build/%)

UNIT_TESTS := $(filter build/src/test/json/%,$(BINARIES))

all: $(BINARIES)
	@echo "Built with '$(CXX) $(TAOCPPJSON_CXXSTD) -Iinclude $(TAOCPPJSON_CPPFLAGS) $(TAOCPPJSON_CXXFLAGS)'."
	@set -e; for T in $(UNIT_TESTS); do echo $$T; $$T > /dev/null; done
	@echo "All $(words $(UNIT_TESTS)) unit tests passed."

clean:
	rm -rf build/*

.SECONDARY:

build/%.d: %.cc Makefile
	@mkdir -p $(@D)
	$(CXX) $(TAOCPPJSON_CXXSTD) -Iinclude $(TAOCPPJSON_CPPFLAGS) -MM -MQ $@ $< -o $@

build/%: %.cc build/%.d
	$(CXX) $(TAOCPPJSON_CXXSTD) -Iinclude $(TAOCPPJSON_CPPFLAGS) $(TAOCPPJSON_CXXFLAGS) $< -o $@

ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEPENDS)
endif
