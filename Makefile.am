# BEGIN: Variables to change.

# additional subdirs which automake should check (default is src and tests)

mysubdirs =

# additional files to add to a distribution

myextradist =

# any additional files to clean up with 'make clean'

mycleanfiles =

# additional files to remove when make maintainer-clean is called

mymaintainercleanfiles = doc

# END: Variables to change
# past here, hopefully, there is no need to edit anything
SUBDIRS = src ${mysubdirs} tests

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE_NAME@-@MAJOR_VERSION@.pc

@PACKAGE_NAME@-@MAJOR_VERSION@.pc: pkgconfig-template.pc
	ln -f pkgconfig-template.pc @PACKAGE_NAME@-@MAJOR_VERSION@.pc

EXTRA_DIST =                            \
    LICENSE                             \
    README.md                           \
    pkgconfig-template.pc.in            \
    ${myextradist}

CLEANFILES =                            \
    ${mycleanfiles}

MAINTAINERCLEANFILES =                  \
    Makefile.in                         \
    acinclude.m4                        \
    aclocal.m4                          \
    configure                           \
    pkgconfig-template.pc               \
    ${mymaintainercleanfiles}

test: check

maintainer-clean-local:
	rm -f config.guess config.sub install-sh ltmain.sh \
	      missing mkinstalldirs depcomp INSTALL *.pc *.tar.gz \
	      .*.out
	rm -rf autom4te.cache
	if test -d doc; then rm -rf doc; fi

.PHONY: doxygen
doxygen:
	@doc_out=`doxygen doxygen.config 2>&1`; \
	doc_ret=$$? ; \
	echo "$$doc_out" ; \
	if test "x$$doc_ret" = "x1" ; then \
	  num=`echo "$$doc_out" | grep "No input read" | wc -l` ; \
	  if test $${num} -eq 0 ; then \
	    echo "error : doxygen reported an error" ; \
	    exit 1 ; \
	  fi ; \
	fi ; \
	mkdir -p doc/html ; \
	if test ! -f "doc/html/index.html" ; then \
	    echo "<html><head><title>@PACKAGE_UNDERLINE@: Main Page</title></head><body><h1>No documentation for @PACKAGE_UNDERLINE@ yet, complain to @PACKAGE_BUGREPORT@</h1></body></html>" > doc/html/index.html ; \
	    fi

.PHONY: memcheck leakcheck
memcheck leakcheck:
	cd tests/ && $(MAKE) $@

# .BEGIN is ignored by GNU make so we can use it as a guard
.BEGIN:
	@echo "error: GNU Make is required, try aliasing gmake to make"
	@exit 1
