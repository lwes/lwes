# BEGIN: Variables to change.

# prefix to use for the flex/bison parser
myparserprefix = lwes

# -d causes yacc/bison to output a header file which is used by lex
# -p prefixes the parser functions with lwes, so we don't have conflicts
AM_YFLAGS = -d -p ${myparserprefix}
# prefix the lex functions so we don't have conflicts with other libs
AM_LFLAGS = -P${myparserprefix} --nounput
LEX_OUTPUT_ROOT = lex.${myparserprefix}

# any additional includes to add to the compile lines

myincludes =

# any additional files to add to the distribution

myextradist = lwes-calculate-max-event-size

# any additional files to clean up with 'make clean'

mycleanfiles = lwes_esf_parser.c \
               lwes_esf_parser_y.c \
               lwes_esf_parser_y.h

# any additional files to clean up with 'make maintainer-clean'

mymaintainercleanfiles = config.h.in

# list of public library header files

myheaderfiles = lwes_types.h \
                lwes_emitter.h \
                lwes_hash.h \
                lwes_listener.h \
                lwes_event.h \
                lwes_event_type_db.h \
                lwes_marshall_functions.h \
                lwes_net_functions.h \
                lwes_time_functions.h

# list of private library header files
# these aren't installed or listed in the library common header

myprivateheaderfiles = lwes_esf_parser_y.h \
                       lwes_esf_parser.h

# include a top level header which includes all other headers
# if set, MUST be of the form @PACKAGEPACKED@.h

mybaseincludeheader = @PACKAGEPACKED@.h

# list of source files comprising shared library

mysourcefiles = lwes_marshall_functions.c \
                lwes_net_functions.c \
                lwes_time_functions.c \
                lwes_types.c \
                lwes_event.c \
                lwes_event_type_db.c \
                lwes_emitter.c \
                lwes_listener.c \
                lwes_esf_parser_y.y \
                lwes_esf_parser.l \
                lwes_hash.c

# sigh ... automake/autoconf/libtool is not smart enough to handle AC_SUBST
# for libtool library names, so you have to fill this out yourself
#
# it should look like: lib@PACKAGE@.la

lib_LTLIBRARIES =							\
  liblwes.la

liblwes_la_SOURCES = ${mysourcefiles}
liblwes_la_LIBADD =
liblwes_la_LDFLAGS = -version-info @MAJOR_VERSION@:@MINOR_VERSION@:0 @GCOV_LTFLAGS@

bin_PROGRAMS = \
  lwes-event-printing-listener \
  lwes-event-counting-listener \
  lwes-filter-listener \
  lwes-event-testing-emitter \
  lwes-esf-validator

bin_SCRIPTS = \
  lwes-calculate-max-event-size

lwes_filter_listener_SOURCES = \
  lwes-filter-listener.c
lwes_filter_listener_LDADD =  \
  lib@PACKAGE@.la

lwes_esf_validator_SOURCES = \
  lwes-esf-validator.c
lwes_esf_validator_LDADD = \
  lib@PACKAGE@.la

lwes_event_printing_listener_SOURCES = \
  lwes-event-printing-listener.c
lwes_event_printing_listener_LDADD =  \
  lib@PACKAGE@.la

lwes_event_counting_listener_SOURCES = \
  lwes-event-counting-listener.c
lwes_event_counting_listener_LDADD = \
  lib@PACKAGE@.la

lwes_event_testing_emitter_SOURCES = \
  lwes-event-testing-emitter.c
lwes_event_testing_emitter_LDADD = \
  lib@PACKAGE@.la

# END: Variables to change
# past here, hopefully, there is no need to edit anything

INCLUDES = ${myincludes}

baseincludedir = $(includedir)/@PACKAGE_NAME@-@MAJOR_VERSION@
baseinclude_HEADERS = ${mybaseincludeheader}

@PACKAGEPACKED@.h: Makefile
	@(echo "/* Automatically generated common header file */";     \
	  echo "";                                                     \
	  perl -le                                                     \
	    'foreach (@ARGV) { print "#include <@PACKAGE_NAME@-@PACKAGE_VERSION@/$$_>"; }'\
	  ${myheaderfiles}) > @PACKAGEPACKED@.h

betterincludedir = $(baseincludedir)/@PACKAGE_NAME@-@PACKAGE_VERSION@

betterinclude_HEADERS = ${myheaderfiles}

noinst_HEADERS = ${myprivateheaderfiles}

EXTRA_DIST =                            \
  ${myprivateheaderfiles}               \
  ${myextradist}

CLEANFILES =                            \
  @PACKAGEPACKED@.h                     \
  *.bb                                  \
  *.bbg                                 \
  *.da                                  \
  *.gcno                                \
  *.gcda                                \
  ${mycleanfiles}

MAINTAINERCLEANFILES =                  \
  Makefile.in                           \
  ${mymaintainercleanfiles}

# .BEGIN is ignored by GNU make so we can use it as a guard
.BEGIN:
	@echo "error: GNU Make is required, try aliasing gmake to make"
	@exit 1
