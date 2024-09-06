noinst_LTLIBRARIES = libhashmap_funcs.la
libhashmap_funcs_la_SOURCES = hashmap.c hash.c hashmap.h
libhashmap_funcs_la_LDFLAGS = -L$(top_builddir)/vector/src
libhashmap_funcs_la_LIBS = $(CODE_COVERAGE_LIBS)
libhashmap_funcs_la_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS) -I$(top_srcdir)/vector/src
libhashmap_funcs_la_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libhashmap_funcs_la_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)

lib_LTLIBRARIES = libhashmap_static.la

# No support for shared libraries with unresolved symbols on windows
if !MINGW
lib_LTLIBRARIES += libhashmap.la
endif

libhashmap_static_la_SOURCES =
libhashmap_static_la_LDFLAGS = -static
libhashmap_static_la_LIBADD = libhashmap_funcs.la $(top_builddir)/vector/src/libvector_static.la
libhashmap_static_la_LIBS = $(CODE_COVERAGE_LIBS)
libhashmap_static_la_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS)
libhashmap_static_la_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libhashmap_static_la_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)

libhashmap_la_SOURCES =
libhashmap_la_LDFLAGS = -shared
libhashmap_la_LIBADD = libhashmap_funcs.la $(top_builddir)/vector/src/libvector.la
libhashmap_la_LIBS = $(CODE_COVERAGE_LIBS)
libhashmap_la_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS)
libhashmap_la_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libhashmap_la_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)

include_HEADERS = hashmap.h hash.h bitset.h
