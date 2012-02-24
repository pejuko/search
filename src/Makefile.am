# only for libraries
LIBTOOL_DEPS = @LIBTOOL_DEPS@

# only for libraries
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

bin_PROGRAMS 	= srch_test
lib_LTLIBRARIES = libsearch.la
include_HEADERS = search.h

libsearch_la_SOURCES = search.c fuzzy.c brute_force.c karp_rabin.c boyer_moore.c
libsearch_la_LIBADD = -lcontainers

srch_test_SOURCES = srch_test.c
srch_test_LDADD = libsearch.la
srch_test_DEPENDENCIES = libsearch.la

