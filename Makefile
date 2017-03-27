# The Art of C++
# Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
# Please see LICENSE for license or visit https://github.com/taocpp/json

.SUFFIXES:
.SECONDARY:

ifeq ($(OS),Windows_NT)
UNAME_S := $(OS)
else
UNAME_S := $(shell uname -s)
endif

# For Darwin (Mac OS X) we assume that the default compiler
# clang++ is used; when $(CXX) is some version of g++, then
# $(CXXSTD) has to be set to -std=c++11 (or newer) so
# that -stdlib=libc++ is not automatically added.

ifeq ($(CXXSTD),)
CXXSTD := -std=c++11
ifeq ($(UNAME_S),Darwin)
CXXSTD += -stdlib=libc++
endif
endif

# Ensure strict standard compliance and no warnings, can be
# changed if desired.

CPPFLAGS ?= -pedantic
CXXFLAGS ?= -Wall -Wextra -Werror -O3

.PHONY: all compile check clean

SOURCES := $(shell find src -name '*.cc')
DEPENDS := $(SOURCES:%.cc=build/%.d)
BINARIES := $(SOURCES:%.cc=build/%)

UNIT_TESTS := $(filter build/src/test/%,$(BINARIES))

all: compile check

compile: $(BINARIES)

check: $(UNIT_TESTS)
	@set -e; for T in $(UNIT_TESTS); do echo $$T; $$T > /dev/null; done

clean:
	@rm -rf build
	@find . -name '*~' -delete

build/%.d: %.cc Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXSTD) -Iinclude $(CPPFLAGS) -MM -MQ $@ $< -o $@

build/%: %.cc build/%.d
	$(CXX) $(CXXSTD) -Iinclude $(CPPFLAGS) $(CXXFLAGS) $< -o $@

ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEPENDS)
endif
