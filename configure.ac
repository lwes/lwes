dnl Process this file with autoconf to produce a configure script.

AC_INIT([lwes], [0.24.0], [lwes-devel@lists.sourceforge.net])
dnl -- we want a header to include in our source files with configure
dnl    info
AC_CONFIG_HEADERS(src/config.h)

AM_INIT_AUTOMAKE([foreign])

dnl Determine the host type for the host specific inclusion below
AC_CANONICAL_HOST

dnl --- Variables I use later on --

SHORT_DESC="The LWES (Light Weight Event System) is a system for sending structured self describing events over multicast.  This is the C library for sending and receiving events."
MAJOR_VERSION=`echo "[$]PACKAGE_VERSION" |
               perl -ne 'm%^(\d+)\.% && print "[$]1"'`
MINOR_VERSION=`echo "[$]PACKAGE_VERSION" |
               perl -ne 'm%^\d+\.(\d+)% && print "[$]1"'`
RELEASE_NUMBER=`echo "[$]PACKAGE_VERSION" |
                perl -ne 'm%^\d+\.\d+\.(\d+)% && print "[$]1"'`
MAJOR_VERSION_UNDERLINE=`echo "[$]MAJOR_VERSION" | perl -pe 'chomp; s/\W/_/g;'`
PACKAGE_UNDERLINE=`echo "[$]PACKAGE_NAME" | perl -pe 'chomp; s/\W/_/g;'`
PACKAGEPACKED=`echo "[$]PACKAGE_NAME" | perl -pe 'chomp; s/\W//g;'`
VERSION_UNDERLINE=`echo "[$]PACKAGE_VERSION" | perl -pe 'chomp; s/\W/_/g;'`

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(RELEASE_NUMBER)
AC_SUBST(SHORT_DESC)

AC_SUBST(MAJOR_VERSION_UNDERLINE)
AC_SUBST(PACKAGE_UNDERLINE)
AC_SUBST(PACKAGEPACKED)
AC_SUBST(VERSION_UNDERLINE)

dnl -- set maintainer mode
AM_MAINTAINER_MODE
AC_SUBST(USE_MAINTAINER_MODE)

dnl -- make sure we have a C compiler
AC_PROG_CC

dnl -- if we are building a library we need libtool
AM_PROG_LIBTOOL

dnl Checks for programs.
AC_PROG_YACC
AM_PROG_LEX

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h limits.h sys/time.h unistd.h getopt.h)
AC_CHECK_HEADER(valgrind/valgrind.h,
                AC_DEFINE([HAVE_VALGRIND_HEADER],
                          [1],
                          [Define to 1 if the <valgrind/valgrind.h> header is on the system]))

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

dnl Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(gettimeofday socket strerror)

dnl Checks for libraries.
dnl Don't know if I need this, but it won't compile if flex is used without it
AC_CHECK_LIB(fl,main)
flex --nounput -V > /dev/null 2>&1
if test $? -eq 0 ; then
  FLEX_EXTRA_ARGS="--nounput"
else
  FLEX_EXTRA_ARGS=""
fi
AC_SUBST(FLEX_EXTRA_ARGS)

dnl These are mostly for solaris
AC_CHECK_LIB(socket,main)
AC_CHECK_LIB(nsl,main)
AC_CHECK_LIB(xnet,main)
AC_CHECK_LIB(resolv,main)

dnl allow for an external gettimeofday function, mostly useful for people have
dnl reimplemented gettimeofday because the system call is slow (FreeBSD 4.11)
AC_ARG_ENABLE(external-gettimeofday,
              [  --enable-external-gettimeofday=<header>  Provide an external definition of the gettimeofday function.  The header pointed to should contain a macro which defines GETTIMEOFDAY(t,tz).  The header will be included in the appropriate place, so that your macro is substituted],
              [AC_DEFINE([HAVE_EXTERNAL_GETTIMEOFDAY], [1], [Define to 1 if there is an external gettimeofday defined])
               AC_DEFINE_UNQUOTED([EXTERNAL_GETTIMEOFDAY_HEADER], "$enable_external_gettimeofday", [Header for external gettime of day])
               ],
              AC_MSG_WARN(using system gettimeofday))

# --- Coverage hooks ---

AC_ARG_ENABLE(coverage,
              [  --enable-coverage      turn on -fprofile-arcs -ftest-coverage],              [case "${enableval}" in
                yes) ENABLE_COVERAGE=1 ;;
                no) ENABLE_COVERAGE=0 ;;
                *) AC_MSG_ERROR(bad value ${enableval} for --enable-cov) ;;
              esac],
              [ENABLE_COVERAGE=2])

if test "x[$]ENABLE_COVERAGE" = "x1"; then
  AC_MSG_WARN(enable coverage)
  CFLAGS="`echo \"[$]CFLAGS\" | perl -pe 's/-O\d+//g;'` -fprofile-arcs -ftest-coverage"
  CXXFLAGS="`echo \"[$]CXXFLAGS\" | perl -pe 's/-O\d+//g;'` -fprofile-arcs -ftest-coverage"
  GCOV_LTFLAGS="-XCClinker -fprofile-arcs -XCClinker -ftest-coverage"
fi

AC_SUBST(ENABLE_COVERAGE)
AC_SUBST(GCOV_LTFLAGS)

# --- Compiler warnings ---
#
# for developer use, enable lots of compile warnings,
# but don't require this generally, because some system's
# header files (BSD) can't handle it
#
# NB: must add -Werror after AC_PROG_CC, etc., so do this last

AC_ARG_ENABLE(hardcore,
              [  --disable-hardcore      turn off -W -Wall -Werror],
              [case "${enableval}" in
                yes) ENABLE_HARDCORE=1 ;;
                no) ENABLE_HARDCORE=0 ;;
                *) AC_MSG_ERROR(bad value ${enableval} for --disable-hardcore) ;;              esac],
              [ENABLE_HARDCORE=1])

if test "x[$]ENABLE_HARDCORE" = "x1"; then
  AC_MSG_WARN(enable hardcore compile warnings)
  if test "x$CXX" = "x"; then
    dnl - only valid for C with newer gcc's
    CPPFLAGS="[$]CPPFLAGS -Wmissing-prototypes"
  fi
  CPPFLAGS="[$]CPPFLAGS -Werror -W -Wall -Wpointer-arith -Wcast-align -Wwrite-strings"
fi


AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tests/Makefile
                 doxygen.config
                 pkgconfig-template.pc])
AC_CONFIG_FILES([tests/test-wrapper.sh],
                [chmod +x tests/test-wrapper.sh])

AC_OUTPUT
