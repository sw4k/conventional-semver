# SPDX-License-Identifier: MIT
CC := $(shell which gcc)
CFLAGS := -D __USE_GNU -O0 -Werror-implicit-function-declaration $(CFLAGS)
CXX := $(shell which g++)
CPPFLAGS := -D __USE_GNU -g -O0 $(CPPFLAGS) -std=gnu++20

SRCDIR := src
OBJDIR := obj
BINDIR := bin

.PHONY: clean all prepare-install install uninstall

build: $(BINDIR)/conventional-semver

rebuild: clean build

$(OBJDIR) $(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

$(OBJDIR)/%.o: $(OBJDIR) $(SRCDIR)/%.cc
	$(CXX) -c $(SRCDIR)/$*.cc $(CPPFLAGS) -o $(OBJDIR)/$(@F)

OBJECTS := \
	$(OBJDIR)/changelog-output-generator.o \
	$(OBJDIR)/commandline-processor.o \
	$(OBJDIR)/configuration.o \
	$(OBJDIR)/git-adapter.o \
	$(OBJDIR)/git-entry-parser.o \
	$(OBJDIR)/git-log-stream.o \
	$(OBJDIR)/logger.o \
	$(OBJDIR)/program.o \
	$(OBJDIR)/semver-output-generator.o

$(BINDIR)/conventional-semver: $(BINDIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(BINDIR)/conventional-semver

