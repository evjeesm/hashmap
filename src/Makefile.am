noinst_LTLIBRARIES = libhashmap_funcs.la
lib_LTLIBRARIES = libhashmap.la libhashmap_static.la

libhashmap_funcs_la_SOURCES = hashmap.c hash.c hashmap.h
libhashmap_funcs_la_CFLAGS = -I../vector/src/
libhashmap_funcs_la_LDFLAGS = -L../vector/src/.libs/

libhashmap_la_SOURCES =
libhashmap_la_LDFLAGS = -shared
libhashmap_la_CFLAGS =
libhashmap_la_LIBADD = libhashmap_funcs.la ../vector/src/libvector.la

libhashmap_static_la_SOURCES =
libhashmap_static_la_LDFLAGS = -static
libhashmap_static_la_CFLAGS =
libhashmap_static_la_LIBADD = libhashmap_funcs.la ../vector/src/libvector_static.la
