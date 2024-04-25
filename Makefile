# SPDX-License-Identifier: MIT
ifeq ($(SEMVER),)
	SEMVER := $(shell conventional-semver)
endif
MARCH := $(shell uname -m)
ifeq ($(MARCH),x86_64)
	ARCH := amd64
else ifeq ($(MARCH),aarch64)
	ARCH := arm64
else
	ARCH := $(MARCH)
endif
SYS := $(shell echo `uname -s` | tr A-Z a-z)
CC := $(shell which gcc)
CFLAGS := -D __USE_GNU -D SEMVER=$(SEMVER) -O0 -Werror-implicit-function-declaration $(CFLAGS)
CXX := $(shell which g++)
CPPFLAGS := -D __USE_GNU -D SEMVER=$(SEMVER) -g -O0 $(CPPFLAGS) -std=gnu++20

SRCDIR := src
OBJDIR := obj
BINDIR := bin
PKGDIR := package
RLSDIR := releases

.PHONY: clean all package-release install uninstall

build: $(BINDIR)/conventional-semver

rebuild: clean build

$(OBJDIR) $(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf $(PKGDIR)

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

package-release: build
	rm -rf $(PKGDIR)
	mkdir $(PKGDIR)
	mkdir -p ./releases
	cp $(BINDIR)/conventional-semver $(PKGDIR)/conventional-semver
	cp LICENSE $(PKGDIR)/LICENSE
	cp README.md $(PKGDIR)/README.md
	tar --gzip --directory $(PKGDIR) -cf $(RLSDIR)/conventional-semver-$(SEMVER)-$(SYS)-$(ARCH).tgz .
	rm -rf $(PKGDIR)

install: build
	cp $(BINDIR)/conventional-semver /usr/bin/conventional-semver

uninstall: 
	rm /usr/bin/conventional-semver
