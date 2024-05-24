ACLOCAL_AMFLAGS = -I m4 -I/usr/share/aclocal
SUBDIRS = vector vector/src src . tests

debug-hash-map-test: ./src/libhashmap.la ./tests/hashmap_test
	LD_LIBRARY_PATH=./src/.libs:/usr/local/lib CK_FORK=no gdb -tui ./tests/.libs/hashmap_test
