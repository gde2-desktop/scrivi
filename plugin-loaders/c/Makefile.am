# C plugin loader

loaderdir = $(libdir)/scrivi/plugin-loaders

AM_CPPFLAGS = \
	-I$(top_srcdir) 						\
	$(SCRIVI_CFLAGS) 						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)					\
	-DSCRIVI_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

loader_LTLIBRARIES = libcloader.la

NOINST_H_FILES = \
	scrivi-plugin-loader-c.h

libcloader_la_SOURCES = \
	scrivi-plugin-loader-c.c 	\
	$(NOINST_H_FILES)

libcloader_la_LDFLAGS = $(LOADER_LIBTOOL_FLAGS)
libcloader_la_LIBADD = $(SCRIVI_LIBS)

-include $(top_srcdir)/git.mk
