
VALGRIND_memcheck_FLAGS = --leak-check=full --track-origins=yes
@VALGRIND_CHECK_RULES@

TESTS = hashmap_test
check_PROGRAMS = hashmap_test

hashmap_test_SOURCES = hashmap_test.c $(top_builddir)/src/hashmap.h
hashmap_test_CFLAGS = @CHECK_CFLAGS@ -I../vector/src
hashmap_test_LDADD = $(top_builddir)/src/libhashmap.la $(top_builddir)/vector/src/libvector.la @CHECK_LIBS@
