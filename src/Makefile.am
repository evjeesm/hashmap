noinst_LTLIBRARIES = libhashmap_funcs.la
lib_LTLIBRARIES = libhashmap.la libhashmap_static.la

libhashmap_funcs_la_SOURCES = hashmap.c hash.c hashmap.h
libhashmap_funcs_la_CFLAGS = -I$(top_srcdir)/vector/src
libhashmap_funcs_la_LDFLAGS = -L$(top_builddir)/vector/src

libhashmap_la_SOURCES =
libhashmap_la_LDFLAGS = -shared
libhashmap_la_CFLAGS =
libhashmap_la_LIBADD = libhashmap_funcs.la $(top_builddir)/vector/src/libvector.la

libhashmap_static_la_SOURCES =
libhashmap_static_la_LDFLAGS = -static
libhashmap_static_la_CFLAGS =
libhashmap_static_la_LIBADD = libhashmap_funcs.la $(top_builddir)/vector/src/libvector_static.la

include_HEADERS = hashmap.h hash.h bitset.h
