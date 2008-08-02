# BEGIN: Variables to change.

# any additional includes to add to the compile lines

myincludes =

# any additional files to add to the distribution

myextradist = testeventtypedb.esf

# any additional files to clean up with 'make clean'

mycleanfiles = test1.out

# any additional files to clean up with 'make maintainer-clean'

mymaintainercleanfiles =

# list of test programs, in dependency order

mytests = \
        testmarshallfuncs \
        testtimefuncs \
        testhashtable \
        testeventtypedb \
        testevent \
        testnetfuncs \
        testemitandlisten \
        testlwes-event-printing-listener \
        testlwes-event-counting-listener \
        testlwes-event-testing-emitter \
        testlwes-calculate-max-event-size

# list of test scripts, in dependency order

myscripttests =

testmarshallfuncs_SOURCES = testmarshallfuncs.c
testmarshallfuncs_LDADD =

testtimefuncs_SOURCES = testtimefuncs.c
testtimefuncs_LDADD =

testhashtable_SOURCES = testhashtable.c
testhashtable_LDADD = ../src/lwes_types.o

testeventtypedb_SOURCES = testeventtypedb.c
testeventtypedb_LDADD = ../src/lwes_types.o \
                        ../src/lwes_hash.o \
                        ../src/lwes_esf_parser.o \
                        ../src/lwes_esf_parser_y.o

testevent_SOURCES = testevent.c
testevent_LDADD = ../src/lwes_types.o \
                  ../src/lwes_hash.o \
                  ../src/lwes_marshall_functions.o \
                  ../src/lwes_esf_parser.o \
                  ../src/lwes_esf_parser_y.o \
                  ../src/lwes_event_type_db.o

testnetfuncs_SOURCES = testnetfuncs.c
testnetfuncs_LDADD = ../src/lwes_types.o

testemitandlisten_SOURCES = testemitandlisten.c
testemitandlisten_LDADD = ../src/lwes_types.o \
                          ../src/lwes_event.o \
                          ../src/lwes_hash.o \
                          ../src/lwes_marshall_functions.o \
                          ../src/lwes_esf_parser.o \
                          ../src/lwes_esf_parser_y.o \
                          ../src/lwes_event_type_db.o \
                          ../src/lwes_net_functions.o \
                          ../src/lwes_time_functions.o

testlwes_event_printing_listener_SOURCES = \
  testlwes-event-printing-listener.c
testlwes_event_printing_listener_LDADD = \
  ../src/liblwes.la

testlwes_event_counting_listener_SOURCES = \
  testlwes-event-counting-listener.c
testlwes_event_counting_listener_LDADD = \
  ../src/liblwes.la

testlwes_event_testing_emitter_SOURCES = \
  testlwes-event-testing-emitter.c
testlwes_event_testing_emitter_LDADD = \
  ../src/liblwes.la

testlwes_calculate_max_event_size_SOURCES = \
  testlwes-calculate-max-event-size.c
testlwes_calculate_max_event_size_LDADD = \
  ../src/liblwes.la

# END: Variables to change
# past here, hopefully, there is no need to edit anything

INCLUDES = -I../src ${myincludes}

check_PROGRAMS = ${mytests}

check_SCRIPTS  = ${myscripttests}

# globally added to all instances of valgrind calls
VALGRIND_OPTS = ${myextravalgrindopts}

# globally added to all memcheck calls
VALGRIND_MEM_OPTS = --tool=memcheck \
                    --num-callers=10 \
                    ${myextravalgrindmemopts}

# globally added to all leakcheck calls
VALGRIND_LEAK_OPTS = --tool=memcheck \
                     --num-callers=10 \
                     --leak-check=yes \
                     --leak-resolution=high \
                     --show-reachable=yes \
                     ${myextravalgrindleakopts}

memcheck: $(check_PROGRAMS)
	@for x in $(check_PROGRAMS);                                     \
	  do                                                             \
	    $(MAKE) memcheck-$$x;                                        \
	  done

leakcheck: $(check_PROGRAMS)
	@for x in $(check_PROGRAMS);                                     \
	  do                                                             \
	    $(MAKE) leakcheck-$$x;                                       \
	  done

memcheck-%: %
	@echo "*****************************************";                \
	echo "MEMCHECK: $<";                                             \
	echo "ARGS: ${VALGRIND_OPTS} ${VALGRIND_MEM_OPTS} ${$<_VALGRIND_MEM_OPTS}";\
	$(LIBTOOL) --mode=execute                                        \
	  valgrind                                                       \
	    ${VALGRIND_OPTS}                                             \
	    ${VALGRIND_MEM_OPTS}                                         \
	    ${$<_VALGRIND_MEM_OPTS} ./$<

leakcheck-%: %
	@echo "*****************************************";               \
	echo "LEAKCHECK: $<";                                           \
	echo "ARGS: ${VALGRIND_OPTS} ${VALGRIND_LEAK_OPTS} ${$<_VALGRIND_LEAK_OPTS}";                                           \
	$(LIBTOOL) --mode=execute                                       \
	  valgrind                                                      \
	    ${VALGRIND_OPTS}                                            \
	    ${VALGRIND_LEAK_OPTS}                                       \
	    ${$<_VALGRIND_LEAK_OPTS}  ./$<

# NB: TESTS are ordered in dependency order

TESTS = $(patsubst %, testwrapper-%, ${mytests}) ${myscripttests}

testwrapper-%: % test-wrapper.sh test-wrapper.sh.in
	@ln -sf test-wrapper.sh $@
	@chmod +x test-wrapper.sh

EXTRA_DIST =                            \
    ${myextradist}

CLEANFILES =                            \
    testwrapper-*                       \
    *.bb                                \
    *.bbg                               \
    *.da                                \
    *.gcno                              \
    *.gcda                              \
    *.gcov                              \
    test-wrapper.sh                     \
    ${mycleanfiles}

MAINTAINERCLEANFILES =                  \
    Makefile.in                         \
    skeletor-tests.am                   \
    ${mymaintainercleanfiles}

# Tell make to ignore these any files that match these targets.
.PHONY: memcheck leakcheck

# .BEGIN is ignored by GNU make so we can use it as a guard
.BEGIN:
	@echo "error: GNU Make is required, try aliasing gmake to make"
	@exit 1
