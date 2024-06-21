

bin_PROGRAMS = two_sum

two_sum_SOURCES = two_sum.c hashmap.h vector.h
two_sum_CFLAGS = -g -I../vector/src -I../src/
two_sum_LDFLAGS = 
two_sum_LDADD = ../src/libhashmap.la ../vector/src/libvector_static.la

debug-two-sum: ../src/libhashmap.la ../vector/src/libvector_static.la
	LD_LIBRARY_PATH=.libs:../src/.libs:/usr/local/lib gdb -tui .libs/two_sum

valgrind-two-sum: ../src/libhashmap.la ../vector/src/libvector_static.la
	LD_LIBRARY_PATH=.libs:../src/.libs:/usr/local/lib valgrind --leak-check=full --track-origins=yes .libs/two_sum
