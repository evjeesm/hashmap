

bin_PROGRAMS = two_sum

two_sum_SOURCES = two_sum.c $(top_srcdir)/src/hashmap.h

two_sum_CFLAGS = -I$(top_srcdir)/vector/src -I$(top_srcdir)/src
two_sum_LDFLAGS = 
two_sum_LDADD = $(top_builddir)/src/libhashmap.la $(top_builddir)/vector/src/libvector_static.la

debug-two-sum: $(top_builddir)/src/libhashmap.la $(top_builddir)/vector/src/libvector_static.la
	LD_LIBRARY_PATH=.libs:../src/.libs:/usr/local/lib gdb -tui .libs/two_sum

valgrind-two-sum: ../src/libhashmap.la ../vector/src/libvector_static.la
	LD_LIBRARY_PATH=.libs:../src/.libs:/usr/local/lib valgrind --leak-check=full --track-origins=yes .libs/two_sum
